#include "PowerUnits.h"


namespace MeasureUnits {


constexpr Power::Power (): milliwatt_(0) {}

constexpr Power::Power (int64_t milliwatt): milliwatt_(milliwatt) {}


constexpr int64_t Power::asMilliwatt() const {

    return milliwatt_;
}

constexpr double Power::asWatt() const {

    return static_cast<double>(milliwatt_) / kMilliwatt;
}


constexpr Power& Power::operator+=(Power other) {

    milliwatt_ += other.milliwatt_;
    return *this;
}

constexpr Power Power::operator+(Power other) const {

    Power tmp = *this;
    tmp += other;
    return tmp;
}


constexpr Power& Power::operator-=(Power other) {

    milliwatt_ -= other.milliwatt_;
    return *this;
}

constexpr Power Power::operator-(Power other) const {

    Power tmp = *this;
    tmp -= other;
    return tmp;
}


template <typename T>
constexpr Power& Power::operator*=(T multiplier) {

    milliwatt_ = static_cast<int64_t>(milliwatt_ * multiplier);
    return *this;
}

template <typename T>
constexpr Power Power::operator*(T multiplier) const {

    Power tmp = *this;
    tmp *= multiplier;
    return tmp;
}

template <typename T>
constexpr Power operator* (T multiplier, Power power) {

    return power * multiplier;
}


constexpr Power milliwatt(int64_t milliwatt) {

    return Power(milliwatt);
}

constexpr Power watt(double watt) {

    return Power(static_cast<int64_t>(watt * Power::kMilliwatt));
}

// For nlohmann::json serialization
inline void to_json(nlohmann::json &json, const Power &power) {

    json["milliwatt"] = power.asMilliwatt();
}

inline void from_json(const nlohmann::json &json, Power &frequency) {

    frequency = milliwatt(json.at("milliwatt").get<int64_t>());
}


}