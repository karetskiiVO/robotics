#include "UDPClientLinux.h"

UDPClient::Res UDPClient::Dial() {
    auto ptr = UDPClientPtr(new UDPClient());
    ptr->fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (ptr->fileDescriptor == -1) {
        return Result<Ptr, ErrorCode>::Err(ErrorCode::SocketCreate);
    }
    return Result<Ptr, ErrorCode>::Ok(ptr);
}

Result<size_t, UDPClient::ErrorCode> UDPClient::SendTo(const IpAddress &ip, const char *buffer, size_t bufferSize) {
    auto result = ip.asSockaddrStruct();
    if (result.is_err()) {
        return Result<size_t, ErrorCode>::Err(ErrorCode::Other);
    }
    struct sockaddr_in socketAddress = result.unwrap();
    ssize_t size = sendto(fileDescriptor, reinterpret_cast<const void*>(buffer), bufferSize, 
        0, reinterpret_cast<const struct sockaddr*>(&socketAddress), sizeof(socketAddress));
    if (size < 0) {
        switch (errno) {
            case EISCONN:
                return Result<size_t, ErrorCode>::Err(ErrorCode::ConnectionTaken);
            case ENOTCONN:
                return Result<size_t, ErrorCode>::Err(ErrorCode::NotConnected);
            case ECONNRESET:
                return Result<size_t, ErrorCode>::Err(ErrorCode::ConnectionReset);
            default:
                return Result<size_t, ErrorCode>::Err(ErrorCode::Other);
        }
    }
    return Result<size_t, ErrorCode>::Ok(size);
}
