#pragma once
#include "UDPClientLinux.h"

class ControllerPort {
public:
    ControllerPort(const IpAddress &dstIp) : ip(dstIp) {}

    struct __attribute__((packed)) ControllerData {
        float velocityLinear;
        float velocityAngular;
    };

    void Loop() {
        udp.SendTo(ip, reinterpret_cast<const char*>(&data), sizeof(data));
    }

    void SetVelocityLinear(float velocity) { data.velocityLinear = velocity; }
    void SetVelocityAngular(float velocity) { data.velocityAngular = velocity; }

    float GetVelocityLinear() const { return data.velocityLinear; }
    float GetVelocityAngular() const { return data.velocityAngular; } 

private:
    UDPClient udp;
    struct ControllerData data;
    const IpAddress ip;
}
