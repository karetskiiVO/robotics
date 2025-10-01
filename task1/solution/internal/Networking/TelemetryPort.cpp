#include "TelemetryPort.h"

bool TelemetryPort::Loop() {
    size_t size = tcp.receiveData(rxBuffer, rxBufferSize);
    size_t j = 0;
    for (size_t i = 0; i < size; i += 4) {
        switch (state) {
            case ParsingStructData:
                if (i == sizeof(data)) {
                    state = ParsingLidarData;
                } else {
                    reinterpret_cast<char*>(&data)[i] = rxBuffer[i]; 
                }
                break;
            case ParsingLidarData:
                if ((i - sizeof(data)) / sizeof(float) == data.lidarDataSize) {
                    state = ParsingOver;
                } else {
                    lidarData->push_back(*reinterpret_cast<float*>(rxBuffer + i)); 
                }
            case ParsingOver:
                state = ParsingStructData;
        }
    }
    return true;
}
