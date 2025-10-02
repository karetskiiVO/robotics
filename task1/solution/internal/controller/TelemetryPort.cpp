#include "TelemetryPort.h"

TelemetryPort::Res TelemetryPort::Create(const IpAddress &ip) {
    auto ptr = TelemetryPort::Ptr(new TelemetryPort());
    ptr->ip = ip;

    auto result = TCPServer::Dial(ptr->ip);
    if (result.is_err()) {
        return Res::Err(ErrorCode::Network);
    }
    ptr->tcpServer = result.unwrap();
    auto resultConnection = ptr->tcpServer->Connect();
    if (resultConnection.is_err()) {
        return Res::Err(ErrorCode::Network);
    }

    ptr->tcp = resultConnection.unwrap();
    ptr->lidarData = std::make_unique<std::vector<float>>();
    return Res::Ok(ptr);
}

Result <const TelemetryPort::TelemetryData*, TelemetryPort::ErrorCode> TelemetryPort::Loop() {
    auto result = tcp->ReceiveData(reinterpret_cast<char*>(&data), sizeof(data));
    if (result.is_err()) {
        return Result<const TelemetryData*, ErrorCode>::Err(ErrorCode::Network);
    }

    if (data.header != header) {
        return Result<const TelemetryData*, ErrorCode>::Err(ErrorCode::Header);
    }

    for (uint32_t i = 0; i < data.lidarDataSize; i++) {
        float value = 0;
        auto result = tcp->ReceiveData(reinterpret_cast<char*>(&value), sizeof(float));
        if (result.is_err()) {
            return Result<const TelemetryData*, ErrorCode>::Err(ErrorCode::Network);
        }
        lidarData->push_back(value);
    }
    return Result<const TelemetryData*, ErrorCode>::Ok(0);
}
