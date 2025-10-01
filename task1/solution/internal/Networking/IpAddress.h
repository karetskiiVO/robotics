#pragma once
#include <arpa/inet.h>
#include <inttypes.h>

#include <string>
#include <stdexcept>

class IpAddress {
public:
    IpAddress(const std::string &ipAddress, uint16_t port) {
       ipAddressStruct.sin_family = AF_INET;
       ipAddressStruct.sin_port = htons(port);
       ipAddressStruct.sin_addr = parseIpString(ipAddress);
    }

    struct sockaddr_in asSockaddrStruct() const { return ipAddressStruct; }

private:
    struct in_addr parseIpString(const std::string &ipAddress) {
        struct in_addr ipAddressStruct = {0};

        if (ipAddress == "localhost") {
            ipAddressStruct.s_addr = localhost;
            return ipAddressStruct;
        }

        if (!inet_pton(AF_INET, ipAddress.c_str(), reinterpret_cast<void*>(&ipAddressStruct))) {
            throw std::invalid_argument("Invalid IP format");
        }
        return ipAddressStruct;
    }

    struct sockaddr_in ipAddressStruct;
    const in_addr_t localhost = 16777343;
};
