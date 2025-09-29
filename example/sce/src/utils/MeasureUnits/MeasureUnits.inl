#include "MeasureUnits.h"


namespace MeasureUnits {


template <typename T>
constexpr Duration operator/ (T dimensionless, Frequency freq) {
    
    return Duration(static_cast<int64_t>(dimensionless * Time::kNanoseconds / freq.asHertz()));
}


}