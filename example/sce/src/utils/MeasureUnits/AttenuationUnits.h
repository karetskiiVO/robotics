#pragma once

#include <cstdint>
#include <cstdlib>
#include <cmath>

#include <nlohmann/json.hpp>

namespace MeasureUnits {

class dB {

public:

    constexpr dB();
    constexpr explicit dB(double dB);

    constexpr double asdB() const;
    inline double asTimes() const;   

    //---Overloaded-Operators---//
    constexpr dB& operator+=(dB other);
    constexpr dB operator+(dB other) const;

    constexpr dB& operator-=(dB other);
    constexpr dB operator-(dB other) const;

    template <typename T>
    constexpr dB& operator*=(T multiplier);

    template <typename T>
    constexpr dB operator*(T multiplier) const;

    constexpr auto operator<=>(const dB &other) const = default;
    constexpr bool operator==(const dB &other) const = default;

private:

    double dB_;
};

template <typename T>
constexpr dB operator*(T multiplier, dB frequency);

constexpr dB times(double times);

// For nlohmann::json serialization
inline void to_json(nlohmann::json &json, const dB &val);
inline void from_json(const nlohmann::json &json, dB &val);


}  // namespace MeasureUnits

#include "AttenuationUnits.inl"
