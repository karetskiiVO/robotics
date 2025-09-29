#pragma once 

#ifndef LINE_h
#define LINE_h

#include <interfaces.h>
#include <emulatedblock.h>
#include <packet.h>
#include <utils/geocoords.h>
#include <utils/MeasureUnits/MeasureUnits.h>
#include <vector>

#ifdef USE_QT_GUI
#include <qtinterfaces.h>
#endif

namespace Components {

class Line final : 
#ifdef USE_QT_GUI
public IQTWidgetConfigurator,
#endif
public Component, public IReceiver, public IConfigurator {
public:
    struct SatelliteData {
        MeasureUnits::Time time;
        Point<int64_t> position; 
        Point<int64_t> velocity; 

        bool operator<(const MeasureUnits::Time& t) const { 
            return time.asNanoseconds() < t.asNanoseconds(); 
        }
    };

    struct Config {
        MeasureUnits::dB attenuation;
        GeoCoordinate coord;          
        int64_t satellite_longitude; 
        std::vector<SatelliteData> satellite_ephemeris;
    };

    Line (Config Config, IBlockCommunicator* nextHop);

    // Non-Copyable and Non-Movable
    // Line(const Line&) = delete;
    // Line& operator=(const Line&) = delete;

    void OnReceive(Packet packet) override;
    void SetConfig(const nlohmann::json& config) override;
    nlohmann::json GetConfig() const override;

#ifdef USE_QT_GUI
    AbstractSettingsWidget* GetSettingsWidget() override;
    QWidget* GetStatsWidget() override;
#endif

    GeoCoordinate CalculateSatellitePosition(MeasureUnits::Time time) const;

public:
    Config config;

private:
    IBlockCommunicator* nextHop;

    GeoCoordinate ExtrapolatePosition(const SatelliteData& data, MeasureUnits::Time currentTime) const;
    GeoCoordinate InterpolatePosition(const SatelliteData& prev, 
                                    const SatelliteData& next, 
                                    MeasureUnits::Time currentTime) const;
};

// Сериализация SatelliteData
inline void to_json(nlohmann::json& json, const Line::SatelliteData& data) {
    json = {
        {"time", data.time.asNanoseconds()},
        {"position_x", data.position.x},
        {"position_y", data.position.y},
        {"position_z", data.position.z},
        {"velocity_x", data.velocity.x},
        {"velocity_y", data.velocity.y},
        {"velocity_z", data.velocity.z}
    };
}

inline void from_json(const nlohmann::json& json, Line::SatelliteData& data) {
    data.time = MeasureUnits::Time(json.at("time").get<int64_t>());
    data.position.x = json.at("position_x").get<int64_t>();
    data.position.y = json.at("position_y").get<int64_t>();
    data.position.z = json.at("position_z").get<int64_t>();
    data.velocity.x = json.at("velocity_x").get<int64_t>();
    data.velocity.y = json.at("velocity_y").get<int64_t>();
    data.velocity.z = json.at("velocity_z").get<int64_t>();
}

} 

#endif // LINE_h