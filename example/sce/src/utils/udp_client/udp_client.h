/**
 * \file
 * \brief Contains udp client for transfering Packets description.
*/

#pragma once

// ============================================================================

#if defined(_WIN32) || defined(_WIN64)
  #include <winsock2.h>
  #include <windows.h>
  #include <mutex>
#elif __linux__
  #include <netinet/in.h>  // sockaddr_in
  using SOCKET = int;
#else
  #error "unknown platform"
#endif

#include <packet.h>
#include <sstream>

// ============================================================================

class UDPClient {
#if defined(_WIN32) || defined(_WIN64)
  std::mutex mutex_;
#endif

 public:
  UDPClient(sockaddr_in host);

  // Non-Copyable
  UDPClient(const UDPClient&) = delete;
  UDPClient& operator=(const UDPClient&) = delete;
  
  UDPClient(UDPClient&&);
  
  UDPClient& operator=(UDPClient&&);

  /// Sends package to specified address.
  /// \throws std::runtime_error if syscall error occurred.
  void Send(const Packet& pkg, sockaddr_in to);

  /// Waits for package until time runs out.
  /// \returns True, if package was received, and false on timeout.
  /// \throws std::runtime_error if syscall error occurred.
  bool Receive(Packet& pkg);

  void SetRecvTimeout(size_t microseconds);

  void Rebind(sockaddr_in new_host);

  void Close();

  SOCKET GetSocket() {
    return sock_;
  }

  sockaddr_in GetHostAddress() const;

  ~UDPClient();

 private:
  void CreateSocket();

  void BindSocket(sockaddr_in host);

  SOCKET sock_;
  sockaddr_in host_;
};

// ============================================================================

class UDPAddr {
  sockaddr_in addr;
public:
  UDPAddr (sockaddr_in addr) : addr(addr) {}
  UDPAddr (const std::string& rawaddr);

  bool operator== (const UDPAddr& other) const;
  bool operator!= (const UDPAddr& other) const;
  bool operator< (const UDPAddr& other) const;

  operator sockaddr_in () { return addr; }

  std::string ToString () const {
    std::stringstream str;
    str << *this;
    return str.str();
  }

  friend std::ostream& operator<< (std::ostream& stream, const UDPAddr& addr);
};

