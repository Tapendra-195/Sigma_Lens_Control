#!/usr/bin/env python3
import os
import sys
import json
import time
import socket
import threading
from datetime import datetime

import numpy as np
import cv2
import zwoasi as asi

SDK_PATH = "/opt/asi/ASI_linux_mac_SDK/lib/armv8/libASICamera2.so"

LISTEN_HOST = "0.0.0.0"
LISTEN_PORT = 5001

DEFAULTS = {
    "cam_index": 0,
    "exposure_us": 30000,
    "gain": 50,
    "format": "tiff",     # tiff|png|jpeg
    "count": 1,
    "prefix": "asi",
    "outdir": "captures",
    "img_type": "raw16",
}


def _ts():
    return datetime.now().strftime("%Y-%m-%d_%H-%M-%S")


def _ensure_dir(path: str) -> str:
    os.makedirs(path, exist_ok=True)
    return path


def _ok(**kwargs):
    d = {"ok": True}
    d.update(kwargs)
    return d


def _err(msg: str, **kwargs):
    d = {"ok": False, "error": msg}
    d.update(kwargs)
    return d


def log(msg: str):
    print(f"[asi_bridge] {msg}", flush=True)


class ASIBridge:
    """
    Phase 1 bridge:
      - TCP JSON line protocol
      - commands: status, set, capture, capture_stream
      - RAW16 capture always
    """

    def __init__(self, sdk_path: str = SDK_PATH):
        self.sdk_path = sdk_path
        self.cam = None
        self.info = None
        self.controls = None
        self.lock = threading.Lock()
        self.state = dict(DEFAULTS)

    def init_camera(self):
        log(f"init_camera(): sdk_path={self.sdk_path}")

        if not os.path.exists(self.sdk_path):
            raise FileNotFoundError(f"ASI SDK library not found: {self.sdk_path}")

        asi.init(self.sdk_path)
        n = asi.get_num_cameras()
        log(f"init_camera(): detected {n} camera(s)")

        if n <= 0:
            raise RuntimeError("No ASI cameras detected")

        cam_index = int(self.state["cam_index"])
        if cam_index < 0 or cam_index >= n:
            raise RuntimeError(f"Camera index {cam_index} out of range (0..{n-1})")

        self.cam = asi.Camera(cam_index)
        self.info = self.cam.get_camera_property()
        self.controls = self.cam.get_controls()

        log(f"init_camera(): opened cam_index={cam_index} name={self.info.get('Name')}")
        log(f"init_camera(): MaxWidth={self.info.get('MaxWidth')} MaxHeight={self.info.get('MaxHeight')}")

        try:
            self.cam.stop_video_capture()
            log("init_camera(): stop_video_capture() ok")
        except Exception as e:
            log(f"init_camera(): stop_video_capture() ignored: {e}")

        try:
            self.cam.stop_exposure()
            log("init_camera(): stop_exposure() ok")
        except Exception as e:
            log(f"init_camera(): stop_exposure() ignored: {e}")

        self.cam.disable_dark_subtract()
        log("init_camera(): dark subtract disabled")

        if "BandWidth" in self.controls:
            bw_min = self.controls["BandWidth"]["MinValue"]
            self.cam.set_control_value(asi.ASI_BANDWIDTHOVERLOAD, bw_min)
            log(f"init_camera(): set bandwidth overload to {bw_min}")
        else:
            log("init_camera(): BandWidth control not present")

        self.apply_settings(
            exposure_us=self.state["exposure_us"],
            gain=self.state["gain"],
        )

    def apply_settings(self, exposure_us=None, gain=None):
        if self.cam is None:
            log("apply_settings(): cam is None, calling init_camera()")
            self.init_camera()

        if exposure_us is not None:
            self.state["exposure_us"] = int(exposure_us)
            self.cam.set_control_value(asi.ASI_EXPOSURE, int(exposure_us))
            log(f"apply_settings(): exposure_us={int(exposure_us)}")

        if gain is not None:
            self.state["gain"] = int(gain)
            self.cam.set_control_value(asi.ASI_GAIN, int(gain))
            log(f"apply_settings(): gain={int(gain)}")

        self.cam.set_image_type(asi.ASI_IMG_RAW16)
        log("apply_settings(): image type set to RAW16")

        time.sleep(0.1)

    def status(self):
        log("status(): called")

        if self.cam is None:
            log("status(): cam is None -> connected=False")
            return _ok(connected=False, sdk_path=self.sdk_path, state=self.state)

        try:
            exp = self.cam.get_control_value(asi.ASI_EXPOSURE)[0]
            gain = self.cam.get_control_value(asi.ASI_GAIN)[0]
        except Exception as e:
            log(f"status(): failed reading controls, using cached values: {e}")
            exp = self.state["exposure_us"]
            gain = self.state["gain"]

        resp = _ok(
            connected=True,
            camera_name=self.info.get("Name"),
            max_width=self.info.get("MaxWidth"),
            max_height=self.info.get("MaxHeight"),
            exposure_us=int(exp),
            gain=int(gain),
            state=self.state,
        )
        log(f"status(): returning connected=True exposure_us={int(exp)} gain={int(gain)}")
        return resp

    def capture(self, count=None, outdir=None, prefix=None, fmt=None):
        if self.cam is None:
            log("capture(): cam is None, calling init_camera()")
            self.init_camera()

        count = int(count if count is not None else self.state["count"])
        fmt = (fmt if fmt is not None else self.state["format"]).lower()
        if fmt == "jpg":
            fmt = "jpeg"
        if fmt not in ("tiff", "png", "jpeg"):
            raise ValueError("format must be one of: tiff, png, jpeg")

        prefix = prefix if prefix is not None else self.state["prefix"]
        outdir = outdir if outdir is not None else self.state["outdir"]

        outdir = os.path.abspath(outdir)
        _ensure_dir(outdir)

        log(f"capture(): count={count} fmt={fmt} outdir={outdir} prefix={prefix}")

        saved = []
        t0 = time.time()

        W = int(self.info["MaxWidth"])
        H = int(self.info["MaxHeight"])
        log(f"capture(): expected frame shape H={H} W={W}")

        for i in range(1, count + 1):
            log(f"capture(): grabbing frame {i}/{count}")
            image_data = self.cam.capture()
            frame16 = np.frombuffer(image_data, dtype=np.uint16).reshape((H, W))

            stamp = _ts()
            filename = os.path.join(outdir, f"{prefix}_{i:03d}_{stamp}.{fmt}")

            if fmt == "jpeg":
                img_to_save = (frame16 / 256).astype("uint8")
            else:
                img_to_save = frame16

            ok = cv2.imwrite(filename, img_to_save)
            if not ok:
                raise RuntimeError(f"cv2.imwrite failed for {filename}")

            saved.append(filename)
            log(f"capture(): saved {filename}")
            time.sleep(0.02)

        dt = round(time.time() - t0, 3)
        log(f"capture(): done in {dt}s")
        return _ok(saved=saved, t_capture_s=dt)

    def capture_stream(self, req, conn: socket.socket):
        if self.cam is None:
            log("capture_stream(): cam is None, calling init_camera()")
            self.init_camera()

        exp = req.get("exposure_us", self.state["exposure_us"])
        gain = req.get("gain", self.state["gain"])
        log(f"capture_stream(): requested exposure_us={exp} gain={gain}")
        self.apply_settings(exposure_us=exp, gain=gain)

        log("capture_stream(): grabbing frame")
        image_data = self.cam.capture()

        try:
            w, h, binning, img_type = self.cam.get_roi_format()
            log(f"capture_stream(): roi_format w={w} h={h} binning={binning} img_type={img_type}")
        except Exception as e:
            log(f"capture_stream(): get_roi_format() failed, using max dims: {e}")
            w = int(self.info["MaxWidth"])
            h = int(self.info["MaxHeight"])
            img_type = asi.ASI_IMG_RAW16

        W = int(w)
        H = int(h)
        expected_nbytes = W * H * 2

        if isinstance(image_data, (bytes, bytearray, memoryview)):
            payload = bytes(image_data)
            log("capture_stream(): capture() returned bytes-like object")
        elif hasattr(image_data, "tobytes"):
            payload = image_data.tobytes(order="C")
            log(f"capture_stream(): capture() returned array-like object type={type(image_data)}")
        else:
            return _err(f"Unexpected capture() return type: {type(image_data)}")

        got_nbytes = len(payload)
        log(f"capture_stream(): got_nbytes={got_nbytes} expected_nbytes={expected_nbytes}")

        if got_nbytes != expected_nbytes:
            return _err(
                f"Unexpected frame size: got {got_nbytes} bytes, expected {expected_nbytes}",
                got=got_nbytes, expected=expected_nbytes, w=W, h=H, img_type=str(img_type)
            )

        header = {
            "ok": True,
            "cmd": "capture_stream",
            "w": W,
            "h": H,
            "pixfmt": "raw16_le",
            "nbytes": expected_nbytes,
            "exposure_us": int(self.state["exposure_us"]),
            "gain": int(self.state["gain"]),
        }

        log(f"capture_stream(): sending header {header}")
        conn.sendall((json.dumps(header) + "\n").encode("utf-8"))
        conn.sendall(payload)
        log("capture_stream(): binary payload sent")

        return None

    def handle_request(self, req: dict, conn: socket.socket):
        cmd = (req.get("cmd") or "").strip().lower()
        log(f"handle_request(): cmd={cmd} req={req}")

        if not cmd:
            return _err("Missing cmd")

        if cmd == "status":
            return self.status()

        if cmd == "set":
            exp = req.get("exposure_us", None)
            gain = req.get("gain", None)
            try:
                self.apply_settings(exposure_us=exp, gain=gain)
            except Exception as e:
                log(f"handle_request(): set failed: {e}")
                return _err(f"Failed to apply settings: {e}")
            return _ok(state=self.state)

        if cmd == "capture":
            try:
                return self.capture(
                    count=req.get("count", None),
                    outdir=req.get("outdir", None),
                    prefix=req.get("prefix", None),
                    fmt=req.get("format", None),
                )
            except Exception as e:
                log(f"handle_request(): capture failed: {e}")
                return _err(f"Capture failed: {e}")

        if cmd == "capture_stream":
            try:
                return self.capture_stream(req, conn)
            except Exception as e:
                log(f"handle_request(): capture_stream failed: {e}")
                return _err(f"Capture failed: {e}")

        return _err(f"Unknown cmd: {cmd}")


def serve():
    bridge = ASIBridge()

    try:
        bridge.init_camera()
        log(f"Camera initialized: {bridge.info.get('Name')}")
    except Exception as e:
        log(f"WARNING: camera not initialized yet: {e}")

    log(f"Listening on {LISTEN_HOST}:{LISTEN_PORT}")
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind((LISTEN_HOST, LISTEN_PORT))
    srv.listen(5)

    while True:
        conn, addr = srv.accept()
        threading.Thread(target=client_thread, args=(bridge, conn, addr), daemon=True).start()


def client_thread(bridge: ASIBridge, conn: socket.socket, addr):
    log(f"Client connected: {addr}")

    # For debugging, do NOT auto-drop an idle connection after 60 s
    conn.settimeout(None)

    buf = b""

    try:
        while True:
            data = conn.recv(4096)
            if not data:
                log(f"Client {addr} closed connection")
                break

            log(f"RX {addr}: {data!r}")
            buf += data

            while b"\n" in buf:
                line, buf = buf.split(b"\n", 1)
                line = line.strip()
                if not line:
                    continue

                log(f"JSON line {addr}: {line!r}")

                try:
                    req = json.loads(line.decode("utf-8"))
                except Exception as e:
                    log(f"Invalid JSON from {addr}: {e}")
                    resp = _err("Invalid JSON (expected one JSON object per line)")
                    conn.sendall((json.dumps(resp) + "\n").encode("utf-8"))
                    continue

                with bridge.lock:
                    resp = bridge.handle_request(req, conn)

                if resp is not None:
                    log(f"TX {addr}: {resp}")
                    conn.sendall((json.dumps(resp) + "\n").encode("utf-8"))

    except Exception as e:
        log(f"Client error {addr}: {e}")
    finally:
        try:
            conn.close()
        except Exception:
            pass
        log(f"Client disconnected: {addr}")


if __name__ == "__main__":
    serve()