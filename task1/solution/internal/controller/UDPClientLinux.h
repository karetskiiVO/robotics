#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <unistd.h>
#include <memory>

#include <rustcxx/rustcxx.hpp>
#include "IpAddress.h"

using namespace rust;

class UDPClient {
public:
    using Ptr = std::shared_ptr<UDPClient>;

    UDPClient() = default;
    ~UDPClient() {
        close(fileDescriptor);
    }

    UDPClient(const UDPClient &other) = delete;
    UDPClient& operator = (const UDPClient&) = delete;

    enum class ErrorCode {
        SocketCreate,
        NotConnected,
        ConnectionTaken,
        ConnectionReset,
        Other
    };

    using Res = Result<Ptr, ErrorCode>;

    static Res Dial();
    Result<size_t, ErrorCode> SendTo(const IpAddress &ip, const char *buffer, size_t bufferSize);

private:
    int fileDescriptor;
};

using UDPClientPtr = UDPClient::Ptr;
