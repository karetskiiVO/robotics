#include "TimeUnits.h"


namespace MeasureUnits {


//====================>Start-of-Time-class<====================//


//---Constructors---//
constexpr Time::Time (): nanoseconds_(0) {}

constexpr Time::Time (int64_t nanosecond): nanoseconds_(nanosecond) {}

//constexpr Time::Time (Duration duration): nanoseconds_(duration.asNanoseconds()) {}


//---Methods---//
constexpr int64_t Time::asNanoseconds () const {

    return nanoseconds_;
}

constexpr double Time::asMicroseconds () const {

    return nanoseconds_ * static_cast<double>(kMicroseconds) / kNanoseconds;
}

constexpr double Time::asMilliseconds () const {

    return nanoseconds_ * static_cast<double>(kMilliseconds) / kNanoseconds;
}

constexpr double Time::asSeconds () const {

    return double(nanoseconds_) / kNanoseconds;
}


//---Overloaded-Operators---//
constexpr Time& Time::operator+= (Duration other) {

    nanoseconds_ += other.asNanoseconds();
    return *this;
}

constexpr Time Time::operator+ (Duration other) const {

    Time tmp = *this;
    tmp += other;
    return tmp;
}

constexpr Time& Time::operator-= (Duration other) {

    nanoseconds_ -= other.asNanoseconds();
    return *this;
}

constexpr Time Time::operator- (Duration other) const {

    Time tmp = *this;
    tmp -= other;
    return tmp;
}

constexpr Duration Time::operator- (Time other) const {

    return Duration(nanoseconds_ - other.nanoseconds_);
}


//For nlohmann::json serialization
void to_json(nlohmann::json& json, const Time &time) {

    json["nanoseconds"] = time.asNanoseconds();
}

void from_json(const nlohmann::json& json, Time &time) {

    time = nanoseconds(json.at("nanoseconds").get<int64_t>());
}


//====================>End-of-Time-class<====================//



//====================>Start-of-Duration-class<====================//


//---Constructors---//
constexpr Duration::Duration (): nanoseconds_(0) {}

constexpr Duration::Duration (int64_t nanoseconds): nanoseconds_(nanoseconds) {}


//---Methods---//
constexpr int64_t Duration::asNanoseconds () const {

    return nanoseconds_;
}

constexpr double Duration::asMicroseconds () const {

    return nanoseconds_ * static_cast<double>(Time::kMicroseconds) / Time::kNanoseconds;
}

constexpr double Duration::asMilliseconds () const {

    return nanoseconds_ * static_cast<double>(Time::kMilliseconds) / Time::kNanoseconds;
}

constexpr double Duration::asSeconds () const {

    return static_cast<double>(nanoseconds_) / Time::kNanoseconds;
}


//---Overloaded-Operators---//
constexpr Duration& Duration::operator+= (Duration other) {

    nanoseconds_ += other.nanoseconds_;
    return *this;
}

constexpr Duration Duration::operator+ (Duration other) const {

    Duration tmp = *this;
    tmp += other;
    return tmp;
}

constexpr Duration& Duration::operator-= (Duration other) {

    nanoseconds_ -= other.nanoseconds_;
    return *this;
}

constexpr Duration Duration::operator- (Duration other) const {

    Duration tmp = *this;
    tmp -= other;
    return tmp;
}

template <typename T>
constexpr Duration& Duration::operator*= (T other) {

    nanoseconds_ *= other;
    return *this;
}

template <typename T>
constexpr Duration Duration::operator* (T other) const {

    Duration tmp = *this;
    tmp *= other;
    return tmp;
}

constexpr Duration operator* (int64_t multiplier, Duration duration) {

    return duration * multiplier;
}


//====================>End-of-Duration-class<====================//


constexpr Duration nanoseconds (int64_t nanoseconds) {

    return Duration(nanoseconds);
}

constexpr Duration microseconds (double microseconds) {

    return Duration(static_cast<int64_t>(microseconds * Time::kNanoseconds / Time::kMicroseconds));
}

constexpr Duration milliseconds (double milliseconds) {

    return Duration(static_cast<int64_t>(milliseconds * Time::kNanoseconds / Time::kMilliseconds));
}

constexpr Duration seconds (double seconds) {

    return Duration(static_cast<int64_t>(seconds * Time::kNanoseconds));
}


//For nlohmann::json serialization
void to_json(nlohmann::json& json, const Duration &duration) {

    json["nanoseconds"] = duration.asNanoseconds();
}

void from_json(const nlohmann::json& json, Duration &duration) {

    duration = nanoseconds(json.at("nanoseconds").get<int64_t>());
}


}
