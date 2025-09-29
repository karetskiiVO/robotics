#include "modcod.h"

namespace ModCod {

FrameData getNormalData(Type type, bool withPilots);

FrameData getShortData(Type type, bool withPilots);

FrameData getFrameData(Type type, FrameThroughputMode mode, bool withPilots) {
    switch (mode) {
        case FrameThroughputMode::NORMAL:
            return getNormalData(type, withPilots);
        case FrameThroughputMode::SHORT:
            return getShortData(type, withPilots);
        default:
            throw std::invalid_argument("Invalid frame throughput mode");
    }
}

FrameData getNormalData(Type type, bool withPilots) {
    uint16_t symbol_length = 0;

    switch (type) {
        case Type::DUMMY_PLFRAME:
            symbol_length = 0;
            break;

        case Type::QPSK_1_4:
        case Type::QPSK_1_3:
        case Type::QPSK_2_5:
        case Type::QPSK_1_2:
        case Type::QPSK_3_5:
        case Type::QPSK_2_3:
        case Type::QPSK_3_4:
        case Type::QPSK_4_5:
        case Type::QPSK_5_6:
        case Type::QPSK_8_9:
        case Type::QPSK_9_10:
            symbol_length = withPilots ? 32490 : 33282;
            break;

        case Type::PSK8_3_5:
        case Type::PSK8_2_3:
        case Type::PSK8_3_4:
        case Type::PSK8_5_6:
        case Type::PSK8_8_9:
        case Type::PSK8_9_10:
            symbol_length = withPilots ? 22194 : 21690;
            break;

        case Type::APSK16_2_3:
        case Type::APSK16_3_4:
        case Type::APSK16_4_5:
        case Type::APSK16_5_6:
        case Type::APSK16_8_9:
        case Type::APSK16_9_10:
            symbol_length = withPilots ? 16686 : 16290;
            break;

        case Type::APSK32_3_4:
        case Type::APSK32_4_5:
        case Type::APSK32_5_6:
        case Type::APSK32_8_9:
        case Type::APSK32_9_10:
            symbol_length = withPilots ? 13338 : 13050;
            break;

        default:
            throw std::out_of_range("Unknown MODCOD type");
    }

    switch (type) {
        case Type::DUMMY_PLFRAME: return {0, symbol_length};
        case Type::QPSK_1_4:    return {2001, symbol_length};
        case Type::QPSK_1_3:    return {2676, symbol_length};
        case Type::QPSK_2_5:    return {3216, symbol_length};
        case Type::QPSK_1_2:    return {4026, symbol_length};
        case Type::QPSK_3_5:
        case Type::PSK8_3_5:    return {4836, symbol_length};
        case Type::QPSK_2_3:
        case Type::PSK8_2_3:
        case Type::APSK16_2_3:  return {5380, symbol_length};
        case Type::QPSK_3_4:
        case Type::PSK8_3_4:
        case Type::APSK16_3_4:
        case Type::APSK32_3_4:  return {6051, symbol_length};
        case Type::QPSK_4_5:
        case Type::APSK16_4_5:
        case Type::APSK32_4_5:  return {6456, symbol_length};
        case Type::QPSK_5_6:
        case Type::PSK8_5_6:
        case Type::APSK16_5_6:
        case Type::APSK32_5_6:  return {6730, symbol_length};
        case Type::QPSK_8_9:
        case Type::PSK8_8_9:
        case Type::APSK16_8_9:
        case Type::APSK32_8_9:  return {7184, symbol_length};
        case Type::QPSK_9_10:
        case Type::PSK8_9_10:
        case Type::APSK16_9_10:
        case Type::APSK32_9_10: return {7274, symbol_length};
        default:
            throw std::out_of_range("Unknown MODCOD type");
    }
}

FrameData getShortData(Type type, bool withPilots) {
    uint16_t symbol_length = 0;

    switch (type) {
        case Type::DUMMY_PLFRAME:
            symbol_length = 0;
            break;

        case Type::QPSK_1_4:
        case Type::QPSK_1_3:
        case Type::QPSK_2_5:
        case Type::QPSK_1_2:
        case Type::QPSK_3_5:
        case Type::QPSK_2_3:
        case Type::QPSK_3_4:
        case Type::QPSK_4_5:
        case Type::QPSK_5_6:
        case Type::QPSK_8_9:
        case Type::QPSK_9_10:
            symbol_length = withPilots ? 8370 : 8190;
            break;

        case Type::PSK8_3_5:
        case Type::PSK8_2_3:
        case Type::PSK8_3_4:
        case Type::PSK8_5_6:
        case Type::PSK8_8_9:
        case Type::PSK8_9_10:
            symbol_length = withPilots ? 5598 : 5490;
            break;

        case Type::APSK16_2_3:
        case Type::APSK16_3_4:
        case Type::APSK16_4_5:
        case Type::APSK16_5_6:
        case Type::APSK16_8_9:
        case Type::APSK16_9_10:
            symbol_length = withPilots ? 4212 : 4140;
            break;

        case Type::APSK32_3_4:
        case Type::APSK32_4_5:
        case Type::APSK32_5_6:
        case Type::APSK32_8_9:
        case Type::APSK32_9_10:
            symbol_length = withPilots ? 3402 : 3330;
            break;

        default:
            throw std::out_of_range("Unknown MODCOD type");
    }

    switch (type) {
        case Type::DUMMY_PLFRAME: return {0, symbol_length};
        case Type::QPSK_1_4:    return {384, symbol_length};
        case Type::QPSK_1_3:    return {654, symbol_length};
        case Type::QPSK_2_5:    return {789, symbol_length};
        case Type::QPSK_1_2:    return {879, symbol_length};
        case Type::QPSK_3_5:
        case Type::PSK8_3_5:    return {1194, symbol_length};
        case Type::QPSK_2_3:
        case Type::PSK8_2_3:
        case Type::APSK16_2_3:  return {1329, symbol_length};
        case Type::QPSK_3_4:
        case Type::PSK8_3_4:
        case Type::APSK16_3_4:
        case Type::APSK32_3_4:  return {1464, symbol_length};
        case Type::QPSK_4_5:
        case Type::APSK16_4_5:
        case Type::APSK32_4_5:  return {1554, symbol_length};
        case Type::QPSK_5_6:
        case Type::PSK8_5_6:
        case Type::APSK16_5_6:
        case Type::APSK32_5_6:  return {1644, symbol_length};
        case Type::QPSK_8_9:
        case Type::PSK8_8_9:
        case Type::APSK16_8_9:
        case Type::APSK32_8_9:  return {1779, symbol_length};
        default:
            throw std::out_of_range("Unknown MODCOD type");
    }
}

}