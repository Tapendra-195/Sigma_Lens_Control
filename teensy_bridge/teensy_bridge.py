#!/usr/bin/env python3
import asyncio, serial, serial.tools.list_ports, sys, time

BAUD = 9600
TCP_PORT = 5000

def find_teensy_port():
    # Prefer stable by-id names if available
    import glob, os
    by_id = glob.glob("/dev/serial/by-id/*")
    for p in by_id:
        if "Teensy" in p or "teensy" in p:
            return p
    # Fallback: scan ports
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        desc = (p.description or "").lower()
        manuf = (p.manufacturer or "").lower()
        if "teensy" in desc or "teensy" in manuf:
            return p.device
    # Last resort: common names
    for p in ["/dev/ttyACM0", "/dev/ttyACM1", "/dev/ttyUSB0"]:
        try:
            open(p).close()
            return p
        except Exception:
            pass
    return None

class Bridge:
    def __init__(self, ser):
        self.ser = ser
        self.clients = set()
        self.ser_lock = asyncio.Lock()

    async def serial_reader(self):
        # Forward whatever Teensy emits to all TCP clients
        loop = asyncio.get_running_loop()
        while True:
            data = await loop.run_in_executor(None, self.ser.read, 1024)
            if data:
                dead = []
                for w in list(self.clients):
                    try:
                        w.write(data)
                        await w.drain()
                    except Exception:
                        dead.append(w)
                for w in dead:
                    self.clients.discard(w)

    async def handle_client(self, reader, writer):
        self.clients.add(writer)
        try:
            while True:
                data = await reader.read(1024)
                if not data:
                    break
                # Forward client bytes to serial exactly (Qt sends newline-terminated commands)
                async with self.ser_lock:
                    self.ser.write(data)
                    self.ser.flush()
        finally:
            self.clients.discard(writer)
            try:
                writer.close()
                await writer.wait_closed()
            except Exception:
                pass

async def main():
    port = find_teensy_port()
    if not port:
        print("ERROR: Could not find Teensy serial port.", file=sys.stderr)
        sys.exit(1)

    ser = serial.Serial(port, BAUD, timeout=0.1)
    print(f"Opened {port} @ {BAUD} baud; serving TCP :{TCP_PORT}")

    bridge = Bridge(ser)
    server = await asyncio.start_server(bridge.handle_client, "0.0.0.0", TCP_PORT)

    async with server:
        await asyncio.gather(server.serve_forever(), bridge.serial_reader())

if __name__ == "__main__":
    asyncio.run(main())