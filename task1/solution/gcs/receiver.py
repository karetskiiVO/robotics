import json
from dataclasses import dataclass
import asyncio


@dataclass
class Telemetry(object):
    odometry_x: float
    odometry_y: float
    odometry_angle: float
    v_x: float
    v_y: float
    omega: float
    omega_x_imu: float
    omega_y_imu: float
    omega_z_imu: float
    lidar_data: list[float]


class TelemetryPort(object):

    def __init__(self, address=('127.0.0.1', 8888)):
        self.ip, self.port = address
        self.telem = None

    async def connect(self):
        loop = asyncio.get_running_loop()
        self.reader, self.writer = asyncio.open_connection(self.ip, self.port,
                                                           loop=loop)

    async def receive_data(self):
        raw_bytes = await self.reader.read(65535)
        data = json.loads(raw_bytes.decode())
        self.telem = Telemetry(**data['Telemetry'])

    @property
    def get_telem(self):
        return self.telem
