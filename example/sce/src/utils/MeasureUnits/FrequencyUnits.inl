#include "FrequencyUnits.h"


namespace MeasureUnits {


//---Constructors---//
constexpr Frequency::Frequency (): hertz_(0) {}

constexpr Frequency::Frequency (int64_t hertz): hertz_(hertz) {}


//---Methods---//
constexpr int64_t Frequency::asHertz () const {

    return hertz_;
}

constexpr double Frequency::asMegahertz () const {

    return static_cast<double>(hertz_) / kMegahertz;
}

constexpr double Frequency::asGigahertz () const {

    return static_cast<double>(hertz_) / kGigahertz;
}


//---Overloaded-Operators---//
constexpr Frequency& Frequency::operator+= (Frequency other) {

    hertz_ += other.hertz_;
    return *this;
}

constexpr Frequency Frequency::operator+ (Frequency other) const {

    Frequency tmp = *this;
    tmp += other;
    return tmp;
}

constexpr Frequency& Frequency::operator-= (Frequency other) {

    hertz_ -= other.hertz_;
    return *this;
}

constexpr Frequency Frequency::operator- (Frequency other) const {

    Frequency tmp = *this;
    tmp -= other;
    return tmp;
}

template <typename T>
constexpr Frequency& Frequency::operator*= (T multiplier) {

    hertz_ = static_cast<int64_t>(hertz_ * multiplier);
    return *this;
}

template <typename T>
constexpr Frequency Frequency::operator* (T multiplier) const {

    Frequency tmp = *this;
    tmp *= multiplier;
    return tmp;
}

template <typename T>
constexpr Frequency operator* (T multiplier, Frequency frequency) {

    return frequency * multiplier;
}


constexpr Frequency hertz (int64_t hertz) {

    return Frequency(hertz);
}

constexpr Frequency megahertz (double megahertz) {

    return Frequency(static_cast<int64_t>(megahertz * Frequency::kMegahertz));
}

constexpr Frequency gigahertz (double gigahertz) {

    return Frequency(static_cast<int64_t>(gigahertz * Frequency::kGigahertz));
}


//For nlohmann::json serialization
void to_json(nlohmann::json& json, const Frequency &frequency) {

    json["hertz"] = frequency.asHertz();
}

void from_json(const nlohmann::json& json, Frequency &frequency) {

    frequency = hertz(json.at("hertz").get<int64_t>());
}


Frequency abs (const Frequency &frequency) {

    return Frequency(std::abs(frequency.asHertz()));
}


}
