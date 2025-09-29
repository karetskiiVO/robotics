#pragma once

#include <cstdint>
#include <stdexcept>

namespace ModCod {

enum class Type {
    DUMMY_PLFRAME = 0,  ///< Заглушка
    QPSK_1_4,           
    QPSK_1_3,           
    QPSK_2_5,           
    QPSK_1_2,           
    QPSK_3_5,           
    QPSK_2_3,           
    QPSK_3_4,           
    QPSK_4_5,           
    QPSK_5_6,           
    QPSK_8_9,           
    QPSK_9_10,          
    PSK8_3_5,           
    PSK8_2_3,           
    PSK8_3_4,           
    PSK8_5_6,           
    PSK8_8_9,           
    PSK8_9_10,          
    APSK16_2_3,         
    APSK16_3_4,         
    APSK16_4_5,         
    APSK16_5_6,         
    APSK16_8_9,         
    APSK16_9_10,        
    APSK32_3_4,         
    APSK32_4_5,         
    APSK32_5_6,         
    APSK32_8_9,         
    APSK32_9_10,        
    RESERVED_29,    ///< Зарезервированный код для будущего расширения    
    RESERVED_30,    ///< Зарезервированный код для будущего расширения
    RESERVED_31     ///< Зарезервированный код для будущего расширения    
};

struct FrameData {
    uint16_t frame_length_bytes;  ///< Длина кадра в байтах.
    uint16_t symbol_length;       ///< Пропускная способность в символах.
};

enum class FrameThroughputMode {
    NORMAL,  ///< Обычный режим работы.
    SHORT    ///< Короткий режим работы.
};


FrameData getFrameData(Type type, FrameThroughputMode mode = FrameThroughputMode::NORMAL, bool withPilots = false);

}