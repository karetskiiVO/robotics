#pragma once

#include <cmath>
#include <cstdint>
#include <numbers>
#include <utils/point.h>

struct GeoCoordinate {
    // Широта в микро-градусах
    int64_t latitude;  

    // Долгота в микро-градусах
    int64_t longitude;  

    // Высота в метрах
    int64_t altitude;       

    GeoCoordinate(int64_t latitude = 0, int64_t longitude = 0, int64_t altitude = 0) :
        latitude(latitude),
        longitude(longitude),
        altitude(altitude) {}

    // Преобразование в декартовы координаты ECEF
    Point<double> toCartesian() const {
        const double semiMajorAxis = 6378137.0; // Большая полуось (м)
        const double eccentricitySquared = 0.00669437999014;

        double lat_rad = (latitude / 1e6) * std::numbers::pi / 180.0;
        double lon_rad = (longitude / 1e6) * std::numbers::pi / 180.0;

        double primeVerticalRadius = semiMajorAxis / 
            std::sqrt(1 - eccentricitySquared * std::sin(lat_rad) * std::sin(lat_rad));

        double x = (primeVerticalRadius + altitude) * std::cos(lat_rad) * std::cos(lon_rad);
        double y = (primeVerticalRadius + altitude) * std::cos(lat_rad) * std::sin(lon_rad);
        double z = (primeVerticalRadius * (1 - eccentricitySquared) + altitude) * std::sin(lat_rad);

        return Point<double>(x, y, z);
    }

    // Расстояние до другой точки в метрах
    int64_t distance(const GeoCoordinate& other) const {
        Point<double> thisPoint = this->toCartesian();
        Point<double> otherPoint = other.toCartesian();
        return static_cast<int64_t>(std::round(thisPoint.Distance(otherPoint)));
    }
};

template<typename T>
GeoCoordinate CartesianToGeo(const Point<T>& cartesian_point) {
    const double semiMajorAxis = 6378137.0;
    const double flattening = 1.0 / 298.257223563; 
    const double semiMinorAxis = semiMajorAxis * (1 - flattening); 
    const double eccentricity = std::sqrt(semiMajorAxis*semiMajorAxis - semiMinorAxis*semiMinorAxis) / semiMajorAxis;
    
    const double x = static_cast<double>(cartesian_point.x);
    const double y = static_cast<double>(cartesian_point.y);
    const double z = static_cast<double>(cartesian_point.z);
    
    const double p = std::sqrt(x*x + y*y);
    const double initialTheta = std::atan2(z * semiMajorAxis, p * semiMinorAxis);
    
    // Первое приближение
    double longitude = std::atan2(y, x);
    double latitude = std::atan2(z + eccentricity * eccentricity * semiMinorAxis * std::pow(std::sin(initialTheta), 3),
                           p - eccentricity * eccentricity * semiMajorAxis * std::pow(std::cos(initialTheta), 3));
    double N = semiMajorAxis / std::sqrt(1 - eccentricity*eccentricity*std::pow(std::sin(latitude), 2));
    double altitude = p / std::cos(latitude) - N;
    
    for (int i = 0; i < 3; ++i) {
        latitude = std::atan2(z, p * (1 - eccentricity*eccentricity*N/(N + altitude)));
        N = semiMajorAxis / std::sqrt(1 - eccentricity*eccentricity*std::pow(std::sin(latitude), 2));
        altitude = p / std::cos(latitude) - N;
    }
    
    // Конвертация в микро-градусы
    return GeoCoordinate(
        static_cast<int64_t>(latitude * 180.0 * 1e6 / std::numbers::pi),
        static_cast<int64_t>(longitude * 180.0 * 1e6 / std::numbers::pi),
        static_cast<int64_t>(altitude)
    );
}

// Сериализация для JSON
inline void to_json(nlohmann::json& json, const GeoCoordinate &coord) {
    json = nlohmann::json {
        {"latitude", coord.latitude},
        {"longitude", coord.longitude},
        {"altitude", coord.altitude}
    };
}

// Десериализация из JSON
inline void from_json(const nlohmann::json& json, GeoCoordinate& coord) {
    json.at("latitude").get_to(coord.latitude);
    json.at("longitude").get_to(coord.longitude);
    json.at("altitude").get_to(coord.altitude);
}