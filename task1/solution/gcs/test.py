import argparse
import json
import asyncio
import struct


class TelemetryReader(object):
    nav_struct = '<II9fI'
    nav_struct_size = struct.calcsize(nav_struct)

    def __init__(self, ip, port, writer):
        self.ip = ip
        self.port = port
        self.writer = writer
        self.conn_evt = asyncio.Event()
        self.reader = None
        self.telem = None

    async def connected_cb(self, reader, writer):
        self.reader = reader
        self.conn_evt.set()

    async def start_server(self):
        self.server = await asyncio.start_server(self.connected_cb, self.ip,
                                                 self.port)
        await self.conn_evt.wait()

    async def receive_telem(self):
        while True:
            raw_bytes = await self.reader.read(65535)
            telem = self.parse_telem(raw_bytes)
            await self.writer.write_telem(telem)

    def parse_telem(self, telem: bytes):
        size, header, odom_x, odom_y, odom_th, vx, vy, vth, wx, wy, wz, n = \
            struct.unpack(TelemetryReader.nav_struct,
                          telem[:TelemetryReader.nav_struct_size])
        lidar_data = struct.unpack(f'<{n}f',
                                   telem[TelemetryReader.nav_struct_size:])
        return dict(Telemetry=dict(odom_x=odom_x, odom_y=odom_y,
                                   odom_th=odom_th, vx=vx, vy=vy, vth=vth,
                                   wx=wx, wy=wy, wz=wz, lidar_data=lidar_data))


class TelemetryWriter(object):

    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.conn_evt = asyncio.Event()

    async def conn_cb(self, reader, writer):
        self.writer = writer
        self.conn_evt.set()

    async def start_server(self):
        self.server = await asyncio.start_server(self.conn_cb, self.ip,
                                                 self.port)
        await self.conn_evt.wait()

    async def write_telem(self, telem: dict):
        raw_bytes = self.encode_telem(telem).encode()
        self.writer.write(raw_bytes)
        await self.writer.drain()

    def encode_telem(self, telem: dict) -> str:
        return json.dumps(telem)


if __name__ == '__main__':
    async def main():
        writer = TelemetryWriter(args.ipout, args.portout)
        reader = TelemetryReader(args.ipin, args.portin, writer)
        await writer.start_server()
        await reader.start_server()

        reader_task = asyncio.create_task(reader.receive_telem())
        await reader_task

    parser = argparse.ArgumentParser(prog='Telemetry bridge',
                                     description='A simple script that '
                                     'retranslates telemetry from raw '
                                     'bytes to JSON data, accessible to GCS.')
    parser.add_argument('--ipin', default='127.0.0.1', help='Telem IP')
    parser.add_argument('--portin', default=5600, help='Telem port')
    parser.add_argument('--ipout', default='127.0.0.1', help='GCS IP')
    parser.add_argument('--portout', default=8888, help='GCS IP')
    args = parser.parse_args()

    asyncio.run(main())
