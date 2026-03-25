# Sigma_Lens_Control

Control software for the Sigma 15mm Art lens (Sony E-mount protocol).

---

## Overview

The system consists of:

- Backend firmware running on a Teensy 4.0  
- Frontend Qt GUI for setting focus and aperture  
- (Optional) TCP bridge running on a Beagle board for network-based control  

---

## Architecture

### Direct USB Mode (Bench Mode)

```
Qt GUI → USB Serial → Teensy → Lens
```

- No network required  
- Recommended for development  

---

### TCP Bridge Mode (Deployment Mode)

```
Qt GUI → TCP → Beagley → USB → Teensy → Lens
```

- Enables remote/headless operation  
- Used for DAQ deployment  

---

# Beagley Setup (TCP Bridge Mode)

## Network Topology

```
DAQ Computer  ←→  Switch  ←→  Beagley  ←→  USB → Teensy + Camera
```

Example IP scheme:

- DAQ: `192.168.50.1`  
- Beagley #1: `192.168.50.2`  
- Beagley #2: `192.168.50.3`  

---

## 1. Configure Beagley Ethernet

Create:

```bash
sudo nano /etc/network/interfaces.d/eth0
```

Add:

```ini
auto eth0
allow-hotplug eth0
iface eth0 inet static
    address 192.168.50.2
    netmask 255.255.255.0
```

---

## 2. Configure DAQ Network

```bash
sudo ip addr add 192.168.50.1/24 dev <interface>
sudo ip link set <interface> up
```

---

## 3. Verify Devices

```bash
lsusb
ls /dev/ttyACM*
```

---

## 4. Auto-Start Services

### Teensy Bridge

Create:

```bash
sudo nano /etc/systemd/system/teensy-bridge.service
```

```ini
[Unit]
Description=Teensy TCP Bridge
After=network.target

[Service]
ExecStart=/usr/bin/python3 /home/photogrammetry/Sigma_Lens_Control/teensy_bridge/teensy_bridge.py
WorkingDirectory=/home/photogrammetry/Sigma_Lens_Control/teensy_bridge
Restart=always
User=photogrammetry

[Install]
WantedBy=multi-user.target
```

Enable:

```bash
sudo systemctl daemon-reload
sudo systemctl enable teensy-bridge
sudo systemctl start teensy-bridge
```

---

### ASI Camera Bridge

Create:

```bash
sudo nano /etc/systemd/system/asi-bridge.service
```

```ini
[Unit]
Description=ASI Camera TCP Bridge
After=network.target

[Service]
ExecStart=/usr/bin/python3 /home/photogrammetry/Sigma_Lens_Control/asi_bridge/asi_bridge.py
WorkingDirectory=/home/photogrammetry/Sigma_Lens_Control/asi_bridge
Restart=always
User=photogrammetry

[Install]
WantedBy=multi-user.target
```

Enable:

```bash
sudo systemctl daemon-reload
sudo systemctl enable asi-bridge
sudo systemctl start asi-bridge
```

---

## 5. Verify Services

```bash
systemctl status teensy-bridge
systemctl status asi-bridge
ss -tlnp | grep 500
```

Expected ports:

- `5000` → Lens (Teensy bridge)  
- `5001` → Camera  

---

## 6. Connect GUI

Lens:

```
tcp:192.168.50.2:5000
```

Camera:

```
tcp:192.168.50.2:5001
```

---

## 7. Troubleshooting

### Network

```bash
ping 192.168.50.2
```

### Logs

```bash
journalctl -u teensy-bridge -f
journalctl -u asi-bridge -f
```

---

## Notes

- Static IPs are required (no DHCP assumed)  
- USB networking (`192.168.7.x`) is only used for initial setup  
- Services auto-restart on failure  

---

## Result

Power on Beagley → system is ready in ~10 seconds.
