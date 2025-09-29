#pragma once

#include <cstdint>
#include <cstdlib>

#include <nlohmann/json.hpp>

namespace MeasureUnits {

class Power {

public:
    constexpr static uint64_t kMilliwatt = 1'000;

public:

    constexpr Power();
    constexpr Power(int64_t milliwatt);

    constexpr int64_t asMilliwatt() const;
    constexpr double asWatt() const;

    constexpr Power& operator+=(Power other);
    constexpr Power operator+(Power other) const;

    constexpr Power& operator-=(Power other);
    constexpr Power operator-(Power other) const;

    template <typename T>
    constexpr Power& operator*=(T multiplier);

    template <typename T>
    constexpr Power operator*(T multiplier) const;

    constexpr auto operator<=>(const Power &other) const = default;
    constexpr bool operator==(const Power &other) const = default;

private:

    int64_t milliwatt_;
};

template <typename T>
constexpr Power operator*(T multiplier, Power power);

constexpr Power milliwatt(int64_t milliwatt);
constexpr Power watt(double watt);

// For nlohmann::json serialization
inline void to_json(nlohmann::json &json, const Power &frequency);
inline void from_json(const nlohmann::json &json, Power &frequency);

}

#include "PowerUnits.inl"