#ifdef __linux__

/**
 * \file
 * \brief Contains udp client implementation.
 */

// ============================================================================

#include <arpa/inet.h>   // inet_addr, inet_ntop
#include <netinet/in.h>  // struct sockaddr_in, htons, ntohs
#include <sys/socket.h>  // socket, bind, sendto, recvfrom
#include <sys/time.h>    // timeval
#include <unistd.h>      // close
#include <cassert>       // assert
#include <cerrno>        // errno
#include <cstdio>        // sprintf
#include <cstring>       // memcmp

#include <utils/udp_client/udp_client.h>

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

    auto total_size = PacketHeader::GetPackedSize() + pkg.Size();

    auto written_bytes = sendto(sock_, buf, total_size, 0,
           reinterpret_cast<sockaddr*>(&to), ADDR_LEN);

    if (size_t(written_bytes) != total_size) {
        throw std::runtime_error("Send failed due to unknown error");
    }
}

bool UDPClient::Receive(Packet& pkg) {
    uint8_t buf[BUFFER_SIZE];

    ssize_t read_bytes = recv(sock_, buf, BUFFER_SIZE, 0);

    if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        }

        throw std::runtime_error("Received failed due to unknown error");
    }

    size_t len = read_bytes;

    if (len < PacketHeader::GetPackedSize()) {
        throw std::runtime_error("Packet doesn't contain header");
    }

    pkg.header.Unpack(buf);
    
    if (PacketHeader::GetPackedSize() + pkg.Size() != len) {
        throw std::runtime_error("Invalid payload size");
    }

    std::vector<uint8_t> payload(pkg.Size());

    memcpy(payload.data(), buf + PacketHeader::GetPackedSize(), pkg.Size());

    pkg.SetPayload(std::move(payload));

    return true;
}

void UDPClient::SetRecvTimeout(size_t microseconds) {
    struct timeval timeout;
    timeout.tv_sec = microseconds / 1000000;
    timeout.tv_usec = microseconds % 1000000;

    int error =
        setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
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
    sock_ = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sock_ >= 0);
}

void UDPClient::BindSocket(sockaddr_in host) {
    // Bind socket to host address

    int error = bind(sock_, reinterpret_cast<const sockaddr*>(&host), ADDR_LEN);
    assert(error >= 0);

    // Retrieve socket info after binding

    socklen_t addr_len = ADDR_LEN;
    getsockname(sock_, reinterpret_cast<sockaddr*>(&host_), &addr_len);
}

void UDPClient::Close() {
    if (sock_ != -1) {
        close(sock_);
    }
}

sockaddr_in UDPClient::GetHostAddress() const { return host_; }

UDPClient::~UDPClient() { Close(); }

// ============================================================================

UDPAddr::UDPAddr(const std::string& rawaddr) {
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    char addr_str[INET_ADDRSTRLEN] = "";
    char port_str[INET_ADDRSTRLEN] = "";

    sscanf(rawaddr.c_str(), "%s:%s", addr_str, port_str);

    inet_ntop(AF_INET, &addr.sin_addr, addr_str, INET_ADDRSTRLEN);

    addr.sin_port = htons(atoi(port_str));
}

std::ostream& operator<<(std::ostream& stream, const UDPAddr& addr) {
    char str[INET_ADDRSTRLEN] = "";

    inet_ntop(AF_INET, &addr.addr.sin_addr, str, INET_ADDRSTRLEN);

    stream << str;

    sprintf(str, "%hu", ntohs(addr.addr.sin_port));

    stream << str;

    return stream;
}

bool UDPAddr::operator<(const UDPAddr& /* unused */) const { return false; }

bool UDPAddr::operator==(const UDPAddr& other) const {
    return this->addr.sin_addr.s_addr == other.addr.sin_addr.s_addr &&
           this->addr.sin_port == other.addr.sin_port;
}

bool UDPAddr::operator!=(const UDPAddr& other) const {
    return !(*this == other);
}

#endif