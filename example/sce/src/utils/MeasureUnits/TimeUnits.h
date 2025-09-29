#pragma once

#include <cstdint>

#include <nlohmann/json.hpp>

namespace MeasureUnits {

class Duration;

class Time {
  public:
    constexpr static uint64_t kNanoseconds = 1'000'000'000;
    constexpr static uint64_t kMicroseconds = 1'000'000;
    constexpr static uint64_t kMilliseconds = 1'000;

  public:
    //---Constructors---//
    constexpr Time();
    constexpr explicit Time(int64_t nanosecond);
    // constexpr explicit Time (Duration duration);

    //---Methods---//
    constexpr int64_t asNanoseconds() const;
    constexpr double asMicroseconds() const;
    constexpr double asMilliseconds() const;
    constexpr double asSeconds() const;

    //---Overloaded-Operators---//
    constexpr Time& operator+=(Duration other);
    constexpr Time operator+(Duration other) const;

    constexpr Time& operator-=(Duration other);
    constexpr Time operator-(Duration other) const;

    constexpr Duration operator-(Time other) const;

    constexpr auto operator<=>(const Time &other) const = default;
    constexpr bool operator==(const Time &other) const = default;

  private:
    int64_t nanoseconds_;
};

// For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const Time& time);
inline void from_json(const nlohmann::json& json, Time& time);

class Duration {
  public:
    //---Constructors---//
    constexpr Duration();
    constexpr explicit Duration(int64_t nanoseconds);

    //---Methods---//
    constexpr int64_t asNanoseconds() const;
    constexpr double asMicroseconds() const;
    constexpr double asMilliseconds() const;
    constexpr double asSeconds() const;

    //---Overloaded-Operators---//
    operator Time() { return Time(nanoseconds_); }

    constexpr Duration& operator+=(Duration other);
    constexpr Duration operator+(Duration other) const;

    constexpr Duration& operator-=(Duration other);
    constexpr Duration operator-(Duration other) const;

    template <typename T>
    constexpr Duration& operator*=(T other);

    template <typename T>
    constexpr Duration operator*(T other) const;

    constexpr auto operator<=>(const Duration &other) const = default;
    constexpr bool operator==(const Duration &other) const = default;

  private:
    int64_t nanoseconds_;
};

constexpr Duration operator*(int64_t multiplier, Duration duration);

constexpr Duration nanoseconds(int64_t nanoseconds);
constexpr Duration microseconds(double microseconds);
constexpr Duration milliseconds(double milliseconds);
constexpr Duration seconds(double seconds);

// For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const Duration& duration);
inline void from_json(const nlohmann::json& json, Duration& duration);

}  // namespace MeasureUnits

#include "TimeUnits.inl"
