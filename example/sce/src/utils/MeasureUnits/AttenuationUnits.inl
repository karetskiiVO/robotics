#include "AttenuationUnits.h"


namespace MeasureUnits {


constexpr dB::dB (): dB_(0) {}

constexpr dB::dB (double dB): dB_(dB) {}


constexpr double dB::asdB () const {

    return dB_;
}

inline double dB::asTimes () const {

    return std::pow(10, dB_ / 10.0);
}

constexpr dB& dB::operator+= (dB other) {

    dB_ += other.dB_;
    return *this;
}

constexpr dB dB::operator+ (dB other) const {

    dB tmp = *this;
    tmp += other;
    return tmp;
}

constexpr dB& dB::operator-= (dB other) {

    dB_ -= other.dB_;
    return *this;
}

constexpr dB dB::operator- (dB other) const {

    dB tmp = *this;
    tmp -= other;
    return tmp;
}

template <typename T>
constexpr dB& dB::operator*= (T multiplier) {

    dB_ = static_cast<double>(dB_ * multiplier);
    return *this;
}

template <typename T>
constexpr dB dB::operator* (T multiplier) const {

    dB tmp = *this;
    tmp *= multiplier;
    return tmp;
}

template <typename T>
constexpr dB operator* (T multiplier, dB frequency) {

    return frequency * multiplier;
}


constexpr dB times(double times) {

    return dB(10 * std::log10(times));
}


//For nlohmann::json serialization
void to_json(nlohmann::json& json, const dB &val) {

    json = nlohmann::json {

        {"db", val.asdB()},
    };
}

void from_json(const nlohmann::json& json, dB &val) {

    val = dB(json.at("dB").get<double>());
}


}