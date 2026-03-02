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
    # Phase 1: keep RAW16 always for capture (jpeg saved as 8-bit downscale)
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


class ASIBridge:
    """
    Phase 1 bridge:
      - TCP JSON line protocol
      - commands: status, set, capture
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
        if not os.path.exists(self.sdk_path):
            raise FileNotFoundError(f"ASI SDK library not found: {self.sdk_path}")

        asi.init(self.sdk_path)

        n = asi.get_num_cameras()
        if n <= 0:
            raise RuntimeError("No ASI cameras detected")

        cam_index = int(self.state["cam_index"])
        if cam_index < 0 or cam_index >= n:
            raise RuntimeError(f"Camera index {cam_index} out of range (0..{n-1})")

        self.cam = asi.Camera(cam_index)
        self.info = self.cam.get_camera_property()
        self.controls = self.cam.get_controls()

        # Stop any ongoing capture
        try:
            self.cam.stop_video_capture()
        except Exception:
            pass
        try:
            self.cam.stop_exposure()
        except Exception:
            pass

        self.cam.disable_dark_subtract()

        # set minimum bandwidth (same as your script)
        if "BandWidth" in self.controls:
            self.cam.set_control_value(
                asi.ASI_BANDWIDTHOVERLOAD,
                self.controls["BandWidth"]["MinValue"],
            )

        # Apply defaults
        self.apply_settings(
            exposure_us=self.state["exposure_us"],
            gain=self.state["gain"],
        )

    def apply_settings(self, exposure_us=None, gain=None):
        if self.cam is None:
            self.init_camera()

        if exposure_us is not None:
            self.state["exposure_us"] = int(exposure_us)
            self.cam.set_control_value(asi.ASI_EXPOSURE, int(exposure_us))

        if gain is not None:
            self.state["gain"] = int(gain)
            self.cam.set_control_value(asi.ASI_GAIN, int(gain))

        # Phase 1: capture always RAW16
        self.cam.set_image_type(asi.ASI_IMG_RAW16)

        time.sleep(0.1)  # let settings settle

    def status(self):
        if self.cam is None:
            # don't auto-init on status; report "not connected" cleanly
            return _ok(connected=False, sdk_path=self.sdk_path, state=self.state)

        try:
            exp = self.cam.get_control_value(asi.ASI_EXPOSURE)[0]
            gain = self.cam.get_control_value(asi.ASI_GAIN)[0]
        except Exception:
            exp = self.state["exposure_us"]
            gain = self.state["gain"]

        return _ok(
            connected=True,
            camera_name=self.info.get("Name"),
            max_width=self.info.get("MaxWidth"),
            max_height=self.info.get("MaxHeight"),
            exposure_us=int(exp),
            gain=int(gain),
            state=self.state,
        )

    def capture(self, count=None, outdir=None, prefix=None, fmt=None):
        if self.cam is None:
            self.init_camera()

        count = int(count if count is not None else self.state["count"])
        fmt = (fmt if fmt is not None else self.state["format"]).lower()
        if fmt == "jpg":
            fmt = "jpeg"
        if fmt not in ("tiff", "png", "jpeg"):
            raise ValueError("format must be one of: tiff, png, jpeg")

        prefix = prefix if prefix is not None else self.state["prefix"]
        outdir = outdir if outdir is not None else self.state["outdir"]

        # If outdir is relative, make it relative to current working directory
        outdir = os.path.abspath(outdir)
        _ensure_dir(outdir)

        saved = []
        t0 = time.time()

        W = int(self.info["MaxWidth"])
        H = int(self.info["MaxHeight"])

        for i in range(1, count + 1):
            image_data = self.cam.capture()  # bytes
            frame16 = np.frombuffer(image_data, dtype=np.uint16).reshape((H, W))

            stamp = _ts()
            filename = os.path.join(outdir, f"{prefix}_{i:03d}_{stamp}.{fmt}")

            if fmt == "jpeg":
                img_to_save = (frame16 / 256).astype("uint8")  # 16-bit -> 8-bit
            else:
                img_to_save = frame16  # PNG/TIFF keep 16-bit

            ok = cv2.imwrite(filename, img_to_save)
            if not ok:
                raise RuntimeError(f"cv2.imwrite failed for {filename}")

            saved.append(filename)
            time.sleep(0.02)

        return _ok(saved=saved, t_capture_s=round(time.time() - t0, 3))

    def capture_stream(self, req, conn: socket.socket):
        if self.cam is None:
            self.init_camera()

        # Apply settings (reuse your existing helper so state stays consistent)
        exp = req.get("exposure_us", self.state["exposure_us"])
        gain = req.get("gain", self.state["gain"])
        self.apply_settings(exposure_us=exp, gain=gain)

        # Capture frame (may be bytes OR numpy array depending on zwoasi build)
        image_data = self.cam.capture()

        # Use the *actual* ROI format, not MaxWidth/MaxHeight
        try:
            w, h, binning, img_type = self.cam.get_roi_format()
        except Exception:
            w = int(self.info["MaxWidth"])
            h = int(self.info["MaxHeight"])
            img_type = asi.ASI_IMG_RAW16

        W = int(w)
        H = int(h)
        expected_nbytes = W * H * 2  # RAW16

        # Convert to raw bytes safely
        if isinstance(image_data, (bytes, bytearray, memoryview)):
            payload = bytes(image_data)
        elif hasattr(image_data, "tobytes"):  # numpy array case
            payload = image_data.tobytes(order="C")
        else:
            return _err(f"Unexpected capture() return type: {type(image_data)}")

        got_nbytes = len(payload)
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

        # Send header (one JSON line), then binary payload
        conn.sendall((json.dumps(header) + "\n").encode("utf-8"))
        conn.sendall(payload)

        # IMPORTANT: return None so client_thread does not send a second JSON line
        return None


    def handle_request(self, req: dict, conn: socket.socket):
        cmd = (req.get("cmd") or "").strip().lower()
        if not cmd:
            return _err("Missing cmd")

        if cmd == "status":
            return self.status()

        if cmd == "set":
            # allow partial sets
            exp = req.get("exposure_us", None)
            gain = req.get("gain", None)
            try:
                self.apply_settings(exposure_us=exp, gain=gain)
            except Exception as e:
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
                return _err(f"Capture failed: {e}")
            
        if cmd == "capture_stream":
            try:
                return self.capture_stream(req, conn)
            except Exception as e:
                return _err(f"Capture failed: {e}")

        return _err(f"Unknown cmd: {cmd}")


def serve():
    bridge = ASIBridge()

    # Try init once at startup, but keep running if no camera present
    try:
        bridge.init_camera()
        print(f"[asi_bridge] Camera initialized: {bridge.info.get('Name')}")
    except Exception as e:
        print(f"[asi_bridge] WARNING: camera not initialized yet: {e}")

    print(f"[asi_bridge] Listening on {LISTEN_HOST}:{LISTEN_PORT}")
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind((LISTEN_HOST, LISTEN_PORT))
    srv.listen(5)

    while True:
        conn, addr = srv.accept()
        # handle each client in a thread (simple)
        threading.Thread(target=client_thread, args=(bridge, conn, addr), daemon=True).start()


def client_thread(bridge: ASIBridge, conn: socket.socket, addr):
    print(f"[asi_bridge] Client connected: {addr}")
    conn.settimeout(60)
    buf = b""

    try:
        while True:
            data = conn.recv(4096)
            if not data:
                break
            buf += data

            while b"\n" in buf:
                line, buf = buf.split(b"\n", 1)
                line = line.strip()
                if not line:
                    continue

                try:
                    req = json.loads(line.decode("utf-8"))
                except Exception:
                    resp = _err("Invalid JSON (expected one JSON object per line)")
                    conn.sendall((json.dumps(resp) + "\n").encode("utf-8"))
                    continue

                # serialize camera access
                with bridge.lock:
                    resp = bridge.handle_request(req, conn)

                # Some commands (e.g. capture_stream) may write directly to conn and return None
                if resp is not None:
                    conn.sendall((json.dumps(resp) + "\n").encode("utf-8"))

    except socket.timeout:
        pass
    except Exception as e:
        print(f"[asi_bridge] Client error {addr}: {e}")
    finally:
        try:
            conn.close()
        except Exception:
            pass
        print(f"[asi_bridge] Client disconnected: {addr}")


if __name__ == "__main__":
    serve()