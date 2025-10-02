#include "ControllerPort.h"

Result<ControllerPort::Ptr, ControllerPort::ErrorCode> ControllerPort::Create(const IpAddress &ip) {
    auto ptr = Ptr(new ControllerPort());
    ptr->ip = ip;

    auto result = UDPClient::Dial();
    if (result.is_err()) {
        return Result<Ptr, ErrorCode>::Err(ErrorCode::Network);
    }
    ptr->udp = result.unwrap();

    return Result<Ptr, ErrorCode>::Ok(ptr);
}

void ControllerPort::Loop() {
    udp->SendTo(ip, reinterpret_cast<const char*>(&data), sizeof(data));
}
