#pragma once
#include <cstddef>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <rustcxx/rustcxx.hpp>

#include "IPAddress.hpp"

namespace net {

using namespace rust;

class TCPConn {
    using Soket = int;
    
    Soket sock;
    TCPConn() = default;
public:
    TCPConn(TCPConn&& other) = default;
    TCPConn& operator=(TCPConn&& other) = default;
    
    TCPConn(const TCPConn&) = delete;
    TCPConn& operator=(const TCPConn&) = delete;

    // TODO: мб сделать Close отдельным методом
    ~TCPConn() {
        close(sock);
    }

    static Result<TCPConn> Dial(const IPAddress& address) {
        auto res = TCPConn();

        Soket sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            return Result<TCPConn>::Err(std::strerror(errno));
        }
        
        res.sock = sock;
        auto bindres = bind(sock, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
        if (bindres == -1) {
            return Result<TCPConn>::Err(std::strerror(errno));
        }

        return Result<TCPConn>::Ok(std::move(res));
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