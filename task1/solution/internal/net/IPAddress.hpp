#pragma once

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <inttypes.h>

#include <netinet/in.h>
#include <string>
#include <rustcxx/rustcxx.hpp>

namespace net {

using namespace rust;

class IPAddress {
    static constexpr in_addr_t localhost = 16777343;
    struct sockaddr_in address;

    IPAddress() = default;
public:
    // TODO: Парсить port из строки
    static Result<IPAddress> Parse(const std::string& ipAddress, uint16_t port) {
        auto res = IPAddress();

        res.address.sin_family = AF_INET;
        res.address.sin_port = htons(port);

        if (ipAddress == "localhost") {
            res.address.sin_addr.s_addr = localhost;
            return Result<IPAddress>::Ok(res);
        }

        if (!inet_pton(AF_INET, ipAddress.c_str(), &res.address)) {
            return Result<IPAddress>::Err(std::strerror(errno));
        }

        return Result<IPAddress>::Ok(res);
    }

    operator sockaddr_in() const { 
        return address;
    }
};

}