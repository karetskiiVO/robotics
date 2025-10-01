#include <sys/socket.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <unistd.h>

#include "IpAddress.h"

class UDPSocket {
public:
    UDPSocket() {
       fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0); 
    }

    ~UDPSocket() {
        close(fileDescriptor);
    }

    UDPSocket(const UDPSocket &other) = delete;
    UDPSocket& operator = (const UDPSocket&) = delete;

    void SendTo(const IpAddress &ip, const char *buffer, size_t bufferSize) {
        struct sockaddr_in socketAddress = ip.asSockaddrStruct();
        (void)sendto(fileDescriptor, reinterpret_cast<const void*>(buffer), bufferSize, 
            0, reinterpret_cast<const struct sockaddr*>(&socketAddress), sizeof(socketAddress));
    }
private:
    int fileDescriptor;
};
