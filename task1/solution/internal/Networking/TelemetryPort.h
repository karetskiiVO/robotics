#include <vector>
#include <memory>
#include <deque>
#include "TCPServerLinux.h"

class TelemetryPort {
public:
    TelemetryPort(const IpAddress &ownIp) : ip(ownIp) {
        tcp.StartServer(ip);
        tcp.Connect();
        state = ParsingOver;
    }

    enum ParserState {
        ParsingStructData,
        ParsingLidarData,
        ParsingOver
    };
    
    struct __attribute__((packed)) TelemetryData {
        uint32_t header;
        float odometryX;
        float odometryY;
        float odometryAngle;
        float velocityX;
        float velocityY;
        float velocityAngular;
        float rollRate;
        float pitchRate;
        float yawRate;
        uint32_t lidarDataSize;
    };

    bool Loop();

private:
    static inline const size_t rxBufferSize = 1024;
    static inline const char header[4] = {0x57, 0x42, 0x54, 0x47};
    const IpAddress ip;

    struct TelemetryData data;
    std::unique_ptr<std::vector<float>> lidarData;
    enum ParserState state;

    TCPServer tcp;
    char rxBuffer[rxBufferSize];
};
