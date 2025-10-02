#pragma once
#include <vector>
#include <memory>
#include <rustcxx/rustcxx.hpp>

#include "TCPServerLinux.h"

using namespace rust;

static const uint32_t header = 1463964743;

class TelemetryPort {
public:
    using Ptr = std::shared_ptr<TelemetryPort>;

    TelemetryPort() = default;
    ~TelemetryPort() = default;

    TelemetryPort(TelemetryPort& other) = delete;
    TelemetryPort& operator = (const TelemetryPort& other) = delete;

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

    enum class ErrorCode {
        Network,
        Header,
        Other
    };

    using Res = Result<Ptr, ErrorCode>;

    static Res Create(const IpAddress &ip);
    Result <const TelemetryData*, ErrorCode> Loop();

    const struct TelemetryData &GetTelemetry() { return data; }

private:
    IpAddress ip;

    struct TelemetryData data;
    std::unique_ptr<std::vector<float>> lidarData;

    TCPServerPtr tcpServer;
    TCPConnectionPtr tcp;
};

using TelemetryPortPtr = TelemetryPort::Ptr;
