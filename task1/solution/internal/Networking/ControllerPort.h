#pragma once
#include <memory>
#include <rustcxx/rustcxx.hpp>
#include "UDPClientLinux.h"

using namespace rust;

class ControllerPort {
public:
    using Ptr = std::shared_ptr<ControllerPort>;

    ControllerPort() = default;
    ~ControllerPort() = default;

    struct __attribute__((packed)) ControllerData {
        float velocityLinear;
        float velocityAngular;
    };

    enum class ErrorCode {
        Network,
        ValueOutOfBounds
    };

    static Result<Ptr, ErrorCode> Create(const IpAddress &ip);

    void Loop();

    // TODO: Добавить проверку значений по границам
    void SetVelocityLinear(float velocity) { data.velocityLinear = velocity; }
    void SetVelocityAngular(float velocity) { data.velocityAngular = velocity; }

    float GetVelocityLinear() const { return data.velocityLinear; }
    float GetVelocityAngular() const { return data.velocityAngular; } 

private:
    UDPClientPtr udp;
    struct ControllerData data;
    IpAddress ip;
};

using ControllerPortPtr = ControllerPort::Ptr;
