#include "TCPServerLinux.h"

TCPServer::Res TCPServer::Dial(const IpAddress &serverIP) {
    auto ptr = Ptr(new TCPServer());
    ptr->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (ptr->fileDescriptor == -1) {
        return Res::Err(ErrorCode::SocketCreate);
    }

    auto result = serverIP.asSockaddrStruct();
    if (result.is_err()) {
        return Res::Err(ErrorCode::InvalidAddress); 
    }
    struct sockaddr_in socketAddress = result.unwrap();

    if (bind(ptr->fileDescriptor, reinterpret_cast<const struct sockaddr*>(&socketAddress), sizeof(socketAddress)) == -1) {
        return Res::Err(ErrorCode::SocketBind);
    }

    if (listen(ptr->fileDescriptor, 1) == -1) {
        return Res::Err(ErrorCode::SocketListen);
    }
    return Res::Ok(ptr);
}

TCPConnection::Res TCPConnection::Create(int socketDescriptor) {
    auto ptr = Ptr(new TCPConnection());

    struct sockaddr_in clientSocketAddress;
    socklen_t clientSocketAddressLen = 0;
    ptr->fileDescriptor = accept(socketDescriptor, 
        reinterpret_cast<struct sockaddr*>(&clientSocketAddress), &clientSocketAddressLen);

    if (ptr->fileDescriptor == -1) {
        return Res::Err(ErrorCode::NotAccepted);
    }
    return Res::Ok(ptr);
}

Result<size_t, TCPConnection::ErrorCode> TCPConnection::ReceiveData(char *buffer, size_t size) {
    ssize_t readSize = read(fileDescriptor, reinterpret_cast<void*>(buffer), size);
    if (readSize == -1) {
        switch (errno) {
            case ECONNRESET:
                return Result<size_t, ErrorCode>::Err(ErrorCode::ConnectionClosed);
            case ENOTCONN:
                return Result<size_t, ErrorCode>::Err(ErrorCode::NotConnected);
            case ETIMEDOUT:
                return Result<size_t, ErrorCode>::Err(ErrorCode::Timeout);
            default:
                return Result<size_t, ErrorCode>::Err(ErrorCode::Other);
        }
    }
    return Result<size_t, ErrorCode>::Ok(static_cast<size_t>(size));
}
    
TCPConnection::Res TCPServer::Connect() {
    return TCPConnection::Create(fileDescriptor);
}
