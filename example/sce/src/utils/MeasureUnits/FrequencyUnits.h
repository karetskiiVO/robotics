#pragma once

#include <cstdint>
#include <cstdlib>

#include <nlohmann/json.hpp>

namespace MeasureUnits {

class Frequency {
  public:
    constexpr static uint64_t kMegahertz = 1'000'000;
    constexpr static uint64_t kGigahertz = 1'000'000'000;

  public:
    //---Constructors---//
    constexpr Frequency();
    constexpr explicit Frequency(int64_t hertz);

    //---Methods---//
    constexpr int64_t asHertz() const;
    constexpr double asMegahertz() const;
    constexpr double asGigahertz() const;

    //---Overloaded-Operators---//
    constexpr Frequency& operator+=(Frequency other);
    constexpr Frequency operator+(Frequency other) const;

    constexpr Frequency& operator-=(Frequency other);
    constexpr Frequency operator-(Frequency other) const;

    template <typename T>
    constexpr Frequency& operator*=(T multiplier);

    template <typename T>
    constexpr Frequency operator*(T multiplier) const;

    constexpr auto operator<=>(const Frequency &other) const = default;
    constexpr bool operator==(const Frequency &other) const = default;

    operator int64_t() { return hertz_; }

  private:
    int64_t hertz_;
};

template <typename T>
constexpr Frequency operator*(T multiplier, Frequency frequency);

constexpr Frequency hertz(int64_t hertz);
constexpr Frequency megahertz(double megahertz);
constexpr Frequency gigahertz(double gigahertz);

// For nlohmann::json serialization
inline void to_json(nlohmann::json &json, const Frequency &frequency);
inline void from_json(const nlohmann::json &json, Frequency &frequency);

inline Frequency abs(const Frequency &frequency);

}  // namespace MeasureUnits

#include "FrequencyUnits.inl"
