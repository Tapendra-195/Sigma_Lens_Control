
# ASI Bridge (asi_bridge)

TCP bridge service for controlling a **ZWO ASI camera** from a remote GUI.

This service runs on the **Beagle (camera node)** and exposes a simple
newline-delimited JSON TCP API for:

* Setting exposure and gain
* Capturing one or more images
* Querying camera status

It is designed to work alongside:

* `teensy_bridge` (lens control, port 5000)
* `asi_bridge` (camera control, port 5001)

Each camera node (Beagle) runs both services.

---

# Architecture

Control PC (Qt GUI)
→ TCP
→ Beagle (camera node)

On each Beagle:

* Lens TCP bridge: **port 5000**
* Camera TCP bridge: **port 5001**

Example:

```
Camera 0 Beagle: 192.168.50.20
  Lens:   tcp:192.168.50.20:5000
  Camera: tcp:192.168.50.20:5001
```

---

# SDK Installation

This bridge requires the **ZWO ASI Linux SDK**.

## 1. Install SDK into standard location

On the Beagle:

```bash
sudo mkdir -p /opt/asi
cd /opt/asi
```

Download the Linux SDK tarball from ZWO (Developer Camera SDK).

Extract:

```bash
sudo tar -xvjf ASI_Camera_SDK.tar.bz2
```

Move to clean versioned folder:

```bash
sudo mv ASI_Camera_SDK/ASI_linux_mac_SDK_V1.41 .
sudo rm -rf ASI_Camera_SDK
```

Create stable symlink:

```bash
sudo ln -sfn /opt/asi/ASI_linux_mac_SDK_V1.41 /opt/asi/ASI_linux_mac_SDK
```

The bridge expects:

```
/opt/asi/ASI_linux_mac_SDK/lib/armv8/libASICamera2.so
```

(For 64-bit ARM systems.)

---

## 2. Install udev rule (IMPORTANT)

Without this, non-root users cannot access the camera.

```bash
sudo cp /opt/asi/ASI_linux_mac_SDK/lib/asi.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Unplug and replug the camera.

---

## 3. Install Python dependencies

```bash
sudo apt update
sudo apt install -y python3-opencv python3-numpy python3-pip
python3 -m pip install zwoasi
```

---

# Service Installation

## 1. Make script executable

```bash
chmod +x asi_bridge.py
```

## 2. Install systemd service

Copy service file:

```bash
sudo cp asi-bridge.service /etc/systemd/system/
```

Reload systemd:

```bash
sudo systemctl daemon-reload
```

Enable at boot:

```bash
sudo systemctl enable asi-bridge.service
```

Start immediately:

```bash
sudo systemctl start asi-bridge.service
```

Check status:

```bash
sudo systemctl status asi-bridge.service
```

View logs:

```bash
journalctl -u asi-bridge.service -f
```

---

# TCP API (Phase 1)

All messages are:

* One JSON object
* One per line
* Newline terminated

---

## Status

Request:

```json
{"cmd":"status"}
```

Response:

```json
{
  "ok": true,
  "connected": true,
  "camera_name": "ASI6200MM Pro",
  "max_width": 9576,
  "max_height": 6388,
  "exposure_us": 30000,
  "gain": 50
}
```

---

## Set Camera Parameters

Request:

```json
{"cmd":"set","exposure_us":30000,"gain":50}
```

Response:

```json
{"ok":true}
```

---

## Capture Images

Request:

```json
{
  "cmd":"capture",
  "count":5,
  "format":"tiff",
  "outdir":"captures/run01",
  "prefix":"cam0"
}
```

Response:

```json
{
  "ok": true,
  "saved": [
    "/abs/path/cam0_001_2026-02-27_14-32-01.tiff"
  ],
  "t_capture_s": 1.42
}
```

---

# Capture Behavior

* Always captures **RAW16**
* TIFF and PNG remain 16-bit
* JPEG is automatically downscaled to 8-bit
* Full sensor resolution is used (Phase 1)
* Images are saved locally on the Beagle

---

# Multi-Camera Deployment

Recommended static IPs:

| Camera ID | IP Address    |
| --------: | ------------- |
|         0 | 192.168.50.20 |
|         1 | 192.168.50.21 |
|         2 | 192.168.50.22 |
|         3 | 192.168.50.23 |
|         4 | 192.168.50.24 |
|         5 | 192.168.50.25 |
|         6 | 192.168.50.26 |
|         7 | 192.168.50.27 |

Control PC: `192.168.50.1`

The Qt GUI can send capture commands to all nodes in parallel for near-simultaneous acquisition.
