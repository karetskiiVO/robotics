#pragma once
#include <vector>
#include <memory>
#include "TCPServerLinux.h"

class TelemetryPort {
public:
    TelemetryPort(const IpAddress &ownIp) : ip(ownIp) {
        tcp.StartServer(ip);
        tcp.Connect();
        lidarData = std::make_unique<std::vector<float>>();
    }

    struct __attribute__((packed)) TelemetryData {
        uint32_t packetSize;
        uint32_t header;
        float odometryX;
        float odometryY;
        float odometryAngle;
        float velocityX;
        float velocityY;
        float velocityAngular;
        float rollRate;
        float pitchRate;
        float yawRate;
        uint32_t lidarDataSize;
    };

    void Loop() {
        (void)tcp.receiveData(reinterpret_cast<char*>(&data), sizeof(data));
        for (uint32_t i = 0; i < data.lidarDataSize; i++) {
            float value = 0;
            (void)tcp.receiveData(reinterpret_cast<char*>(&value), sizeof(float));
            lidarData->push_back(value);
        }
    }

    const struct TelemetryData &GetTelemetry() { return data; }

private:
    const IpAddress ip;

    struct TelemetryData data;
    std::unique_ptr<std::vector<float>> lidarData;

    TCPServer tcp;
};
