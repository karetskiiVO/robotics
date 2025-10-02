#pragma once
#include <arpa/inet.h>
#include <inttypes.h>

#include <string>
#include <stdexcept>
#include <rustcxx/rustcxx.hpp>

using namespace rust;

 static const in_addr_t localhost = 16777343;

class IpAddress {
public:
    IpAddress() = default;
    ~IpAddress() = default;

    IpAddress(IpAddress &other) = default;
    IpAddress& operator = (const IpAddress &other) = default;

    bool Parse(const std::string &ipAddress, uint16_t port) {
       ipAddressStruct.sin_family = AF_INET;
       ipAddressStruct.sin_port = htons(port);

       auto result = parseIpString(ipAddress);
       if (result.is_err()) {
           success = false;
       } else {
           ipAddressStruct.sin_addr = result.unwrap();
           success = true;
       }
       return success;
    }

    Result<struct sockaddr_in, int> asSockaddrStruct() const { 
        if (success) {
            return Result<struct sockaddr_in, int>::Ok(ipAddressStruct); 
        } else {
            return Result<struct sockaddr_in, int>::Err(0);
        }
    }

private:
    Result<struct in_addr, int> parseIpString(const std::string &ipAddress) {
        struct in_addr ipAddressStruct = {0};

        if (ipAddress == "localhost") {
            ipAddressStruct.s_addr = localhost;
            return Result<struct in_addr, int>::Ok(ipAddressStruct);
        }

        if (!inet_pton(AF_INET, ipAddress.c_str(), reinterpret_cast<void*>(&ipAddressStruct))) {
            return Result<struct in_addr, int>::Err(0);
        }
        return Result<struct in_addr, int>::Ok(ipAddressStruct);
    }

    struct sockaddr_in ipAddressStruct;
    bool success;
};
