#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <unistd.h>

#include <rustcxx/rustcxx.hpp>
#include "IPAddress.hpp"

namespace net {

using namespace rust;

class UDPConn {
public:
    using Soket = int;
    
    Soket sock;
    UDPConn() = default;
public:
    UDPConn(UDPConn&& other) = default;
    UDPConn& operator=(UDPConn&& other) = default;
    
    UDPConn(const UDPConn&) = delete;
    UDPConn& operator=(const UDPConn&) = delete;
    
    // TODO: мб сделать Close отдельным методом
    ~UDPConn() {
        close(sock);
    }

    static Result<UDPConn> Dial(const IPAddress& address) {
        auto res = UDPConn();

        Soket sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == -1) {
            return Result<UDPConn>::Err(std::strerror(errno));
        }
        
        res.sock = sock;
        auto bindres = bind(sock, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
        if (bindres == -1) {
            return Result<UDPConn>::Err(std::strerror(errno));
        }

        return Result<UDPConn>::Ok(std::move(res));
    }

    Result<size_t> Write(const void* data, const size_t size) const {
        auto res = send(sock, data, size, 0);
        if (res < 0) {
            return Result<size_t>::Err(std::strerror(errno));
        }

        return Result<size_t>::Ok(static_cast<size_t>(res)); 
    }

    // TODO: Добавить автосериализацию
    template<typename T>
    Result<size_t> Write(const T& data) const {
        return Write(
            reinterpret_cast<const void*>(&data),
            sizeof(T)
        );
    }

    Result<size_t> Read(void* data, const size_t size) const {
        auto res = recv(sock, data, size, 0);
        if (res < 0) {
            return Result<size_t>::Err(std::strerror(errno));
        }

        return Result<size_t>::Ok(static_cast<size_t>(res)); 
    }

    // TODO: Добавить автосериализацию
    template<typename T>
    Result<size_t> Read(T& data) const {
        return Read(
            reinterpret_cast<void*>(&data),
            sizeof(T)
        );
    }
};

}