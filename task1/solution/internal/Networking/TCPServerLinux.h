#pragma once
#include <memory>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <rustcxx/rustcxx.hpp>

#include "IpAddress.h"

using namespace rust;

class TCPConnection {
public:
    using Ptr = std::shared_ptr<TCPConnection>;

    TCPConnection() = default;
    ~TCPConnection() {
        if (fileDescriptor != -1) {
            close(fileDescriptor);
        }
    }

    enum class ErrorCode {
        NotAccepted,
        NotConnected,
        ConnectionClosed,
        Timeout,
        Other
    };

    using Res = Result<Ptr, ErrorCode>;

    TCPConnection(const TCPConnection &other) = delete;
    TCPConnection operator = (const TCPConnection &other) = delete;

    static Res Create(int socketDescriptor);
    Result<size_t, ErrorCode> ReceiveData(char *buffer, size_t size);

private:
    int fileDescriptor;
};

using TCPConnectionPtr = TCPConnection::Ptr;

class TCPServer {
public:
    using Ptr = std::shared_ptr<TCPServer>;
    TCPServer() = default;
    ~TCPServer() {
        if (fileDescriptor != -1) {
            close(fileDescriptor);
        }
    }

    TCPServer(const TCPServer &other) = delete;
    TCPServer& operator = (const TCPServer &other) = delete;

    enum class ErrorCode {
        SocketCreate,
        SocketBind,
        SocketListen,
        SocketAccept,
        SocketReceive,
        InvalidAddress
    };

    using Res = Result<Ptr, ErrorCode>;

    static Res Dial(const IpAddress &serverIP);
    TCPConnection::Res Connect();
    size_t receiveData(char *buffer, size_t size);

private:
    int fileDescriptor;
    int connectionDescriptor;
};

using TCPServerPtr = TCPServer::Ptr;
