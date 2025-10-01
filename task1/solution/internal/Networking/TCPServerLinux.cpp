#include "TCPServerLinux.h"
#include <ios>

void TCPServer::StartServer(const IpAddress &serverIp) {
    struct sockaddr_in socketAddress = serverIp.asSockaddrStruct();
    if (bind(fileDescriptor, reinterpret_cast<const struct sockaddr*>(&socketAddress), sizeof(socketAddress)) == -1) {
        throw std::ios_base::failure("Could not bind to socket");
    }
}

void TCPServer::Connect() {
    if (listen(fileDescriptor, 1) == -1) {
        throw std::ios_base::failure("Could not switch socket to listening mode");
    }
    struct sockaddr_in clientSocketAddress;
    socklen_t clientSocketAddressLen = 0;
    connectionDescriptor = accept(fileDescriptor, reinterpret_cast<struct sockaddr*>(&clientSocketAddress), &clientSocketAddressLen);
    
    if (connectionDescriptor == -1) {
        throw std::ios_base::failure("Could not open connection");
    }
}

size_t TCPServer::receiveData(char *buffer, size_t size) {
    ssize_t readSize = read(connectionDescriptor, reinterpret_cast<void*>(buffer), size);
    if (readSize < 0) {
        throw std::ios_base::failure("Could not read TCP connection!");
    }
    return static_cast<size_t>(readSize);
}
