#define _CRT_SECURE_NO_WARNINGS

#include <utils/udp_client/udp_client.h>

#ifndef __linux__  // defined(_WIN32) || defined(_WIN64)
/**
 * \file
 * \brief Contains udp client implementation.
 */

// ============================================================================

#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#include <cassert>  // assert
#include <cerrno>   // errno
#include <cstdio>   // sprintf
#include <cstring>  // memcmp
#include <iostream>

// ============================================================================

/// Max header + payload size allowed.
static constexpr size_t BUFFER_SIZE = PacketHeader::GetPackedSize() + 4096ULL;

static constexpr size_t ADDR_LEN = sizeof(sockaddr_in);

// ============================================================================

UDPClient::UDPClient(sockaddr_in host) {
    CreateSocket();
    BindSocket(host);
}

void UDPClient::Send(const Packet& pkg, sockaddr_in to) {
   uint8_t buf[BUFFER_SIZE];

    pkg.header.Pack(buf);
    
    memcpy(
        buf + PacketHeader::GetPackedSize(),
        pkg.payload->data(),
        pkg.Size()
    );

    int total_size = static_cast<int>(PacketHeader::GetPackedSize() + pkg.Size());

    sendto(sock_, reinterpret_cast<const char*>(buf), total_size, 0,
           reinterpret_cast<sockaddr*>(&to), ADDR_LEN);
}

bool UDPClient::Receive(Packet& pkg) {
    std::lock_guard lock(mutex_);

    uint8_t buf[BUFFER_SIZE];

    int received_len = -1;
    if (sock_ == INVALID_SOCKET) {
        throw std::runtime_error("Invalid socket");
    }

    received_len = recv(sock_, reinterpret_cast<char*>(buf), int(BUFFER_SIZE), 0);

    // Check for timeout

    if (received_len == -1) {
        auto WSAErr = WSAGetLastError();

        if (WSAErr == WSAEMSGSIZE || WSAErr == WSAETIMEDOUT) return false;

        throw std::runtime_error(
            std::format(
                "error in received_len expected: {} actual: {} wsaerr: {}", 
                PacketHeader::GetPackedSize(), 
                received_len, 
                WSAErr
            )
        );
    }

    // Receive Packet payload

    pkg.header.Unpack(buf);

    std::vector<uint8_t> payload(pkg.Size());

    memcpy(payload.data(), buf + PacketHeader::GetPackedSize(), pkg.Size());

    pkg.SetPayload(std::move(payload));

    return true;
}

void UDPClient::SetRecvTimeout(size_t microseconds) {
    DWORD timeout_ms = static_cast<DWORD>(microseconds / 1000);

    int error = setsockopt(
        sock_, 
        SOL_SOCKET, 
        SO_RCVTIMEO,   
        reinterpret_cast<const char*>(&timeout_ms),
        sizeof(timeout_ms)
    );

    assert(error != -1);
}

void UDPClient::Rebind(sockaddr_in new_host) {
    if (memcmp(&new_host, &host_, sizeof(new_host)) != 0) {
        Close();
        CreateSocket();
        BindSocket(new_host);
    }
}

void UDPClient::CreateSocket() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::runtime_error("can't wsa startup");

    sock_ = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sock_ >= 0);
}

void UDPClient::BindSocket(sockaddr_in host) {
    // Bind socket to host address

    int error = bind(sock_, reinterpret_cast<const sockaddr*>(&host), ADDR_LEN);
    assert(error >= 0);

    // Retrieve socket info after binding

    int addr_len = ADDR_LEN;
    getsockname(sock_, reinterpret_cast<sockaddr*>(&host_), &addr_len);
}

void UDPClient::Close() {
    if (sock_ != -1) {
        closesocket(sock_);
    }
    WSACleanup();
}

sockaddr_in UDPClient::GetHostAddress() const { return host_; }

UDPClient::~UDPClient() { Close(); }

// ============================================================================

UDPAddr::UDPAddr(const std::string& rawaddr) {
    uint16_t port = 0;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    sscanf(rawaddr.c_str(), "%hhu.%hhu.%hhu.%hhu:%hu",
           &addr.sin_addr.S_un.S_un_b.s_b1, &addr.sin_addr.S_un.S_un_b.s_b2,
           &addr.sin_addr.S_un.S_un_b.s_b3, &addr.sin_addr.S_un.S_un_b.s_b4,
           &port);

    addr.sin_port = htons(port);
}

std::ostream& operator<<(std::ostream& stream, const UDPAddr& addr) {
    const uint8_t* ipbytes =
        reinterpret_cast<const uint8_t*>(&addr.addr.sin_addr);

    stream << unsigned(ipbytes[0]) << "." << unsigned(ipbytes[1]) << "."
           << unsigned(ipbytes[2]) << "." << unsigned(ipbytes[3]) << ":"
           << unsigned(htons(addr.addr.sin_port));

    return stream;
}

bool UDPAddr::operator<(const UDPAddr& other) const {
    if (this->addr.sin_addr.S_un.S_addr == other.addr.sin_addr.S_un.S_addr) {
        return this->addr.sin_port < other.addr.sin_port;
    }
    return this->addr.sin_addr.S_un.S_addr < other.addr.sin_addr.S_un.S_addr;
    ;
}

bool UDPAddr::operator==(const UDPAddr& other) const {
    return this->addr.sin_addr.S_un.S_addr == other.addr.sin_addr.S_un.S_addr &&
           this->addr.sin_port == other.addr.sin_port;
}

bool UDPAddr::operator!=(const UDPAddr& other) const {
    return !(*this == other);
}
#endif