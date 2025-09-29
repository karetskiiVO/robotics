#pragma once


#include "TimeUnits.h"
#include "FrequencyUnits.h"
#include "PowerUnits.h"
#include "AttenuationUnits.h"


namespace MeasureUnits {

template <typename T>
constexpr Duration operator/ (T dimensionless, Frequency freq);

}

#include "MeasureUnits.inl"