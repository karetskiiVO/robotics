import json
from dataclasses import dataclass
import asyncio


@dataclass
class Telemetry(object):
    odom_x: float
    odom_y: float
    odom_th: float
    vx: float
    vy: float
    vth: float
    wx: float
    wy: float
    wz: float
    lidar_data: list[float]


class TelemetryPort(object):

    def __init__(self, address=('127.0.0.1', 8888)):
        self.ip, self.port = address
        self.telem = None

    async def connect(self):
        self.reader, self.writer = await asyncio.open_connection(self.ip,
                                                                 self.port)

    async def receive_data(self):
        while True:
            raw_bytes = await self.reader.read(65535)
            data = json.loads(raw_bytes.decode())
            self.telem = Telemetry(**data['Telemetry'])
            print(self.telem)

    @property
    def get_telem(self):
        return self.telem


if __name__ == '__main__':
    async def main():
        port = TelemetryPort()
        await port.connect()

        receive_task = asyncio.create_task(port.receive_data())
        await receive_task

    asyncio.run(main())
