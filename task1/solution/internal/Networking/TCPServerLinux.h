#include <sys/socket.h>
#include <unistd.h>
#include <ios>

#include "IpAddress.h"

class TCPServer {
public:
    TCPServer() {
        fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (fileDescriptor == -1) {
            throw std::ios_base::failure("Could not create TCP socket");
        }
    }

    ~TCPServer() {
        close(fileDescriptor);
    }

    TCPServer(const TCPServer &other) = delete;
    TCPServer& operator = (const TCPServer &other) = delete;

    void StartServer(const IpAddress &serverIp);
    void Connect();
    size_t receiveData(char *buffer, size_t size);

private:
    int fileDescriptor;
    int connectionDescriptor;
};
