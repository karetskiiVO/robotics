#include "TCPServerLinux.h"

class TelemetryPort {
public:
    TelemetryPort(const IpAddress &ownIp) : ip(ownIp) {}
    
    struct __attribute__((packed)) TelemetryData {
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

private:
    const IpAddress ip;
};
