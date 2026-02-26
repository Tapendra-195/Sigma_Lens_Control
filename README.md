## Sigma_Lens_Control

Control software for the Sigma 15mm Art lens (Sony E-mount protocol).

The system consists of:

Backend firmware running on a Teensy 4.0

Frontend Qt GUI for setting focus and aperture

(Optional) a TCP bridge running on a Beagle board for network-based control

The Sigma lens communicates using Sony E-mount protocol.
The Teensy firmware handles low-level protocol communication.
The Qt frontend provides user control.

## Architecture

Two connection modes are supported:

## Direct USB Mode (Original / Bench Mode)
Qt GUI  →  USB Serial  →  Teensy  →  Lens

The GUI connects directly to the Teensy over USB.

Recommended for development and bench testing.

No network required.

##  TCP Bridge Mode (Deployment / Remote Mode)
Qt GUI  →  TCP  →  Beagle (teensy_bridge)  →  USB Serial  →  Teensy  →  Lens

The Teensy is connected to a Beagle board via USB.

The Beagle runs a small TCP bridge (teensy_bridge.py).

The GUI connects to the Beagle over the network.

Useful for remote or headless deployments.

Both modes use the same GUI codebase.
