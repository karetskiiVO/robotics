/**
 * \file
 * \brief Contains input class declaration.
 */

#pragma once

// ============================================================================

#include <interfaces.h>
#include <packet.h>
#include <emulatedblock.h>

#ifdef USE_QT_GUI

#include <qtinterfaces.h>

#endif

// ============================================================================

namespace Components {

/// Generates packages with distinct ids.
class Input final :
#ifdef USE_QT_GUI
public IQTWidgetConfigurator,
#endif
public Component, public ITimeStamper, public IConfigurator {
  public:
    struct Config {
        PacketHeader::Id first_id;              /// Id of the first generated package.
        MeasureUnits::Duration interval;        /// Duration between sending packages (in ticks).
        ModCod::Type modcod;                    /// Modcod of the generated packages.
        size_t data_size;                       /// Data size of the generated packages.

        bool operator== (const Config &other) const = default;
    };

    /// Creates input module.
    Input(Config Config, IBlockCommunicator* next_hop);

    /// Non-Copyable and Non-Movable
    // Input(const Input&) = delete;
    // Input& operator=(const Input&) = delete;

    /// Sends packages with configured speed.
    void OnTimeStamp(MeasureUnits::Time) override;

    /// Configures module via JSON.
    void SetConfig (const nlohmann::json& config) override;
    
    /// Returns configuration in JSON format.
    nlohmann::json GetConfig () const override;

#ifdef USE_QT_GUI
    /// Returns settings widget.
    AbstractSettingsWidget* GetSettingsWidget() override;

    /// Returns widget for displaying statistics.
    QWidget* GetStatsWidget() override;
#endif

public:

    Config config;

private:
    
    PacketHeader::Id next_id_;
    IBlockCommunicator* next_hop_ = nullptr;
    MeasureUnits::Duration time_left_;
};


//For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const Input::Config &config) {

    json = nlohmann::json {

        {"first_id",  config.first_id},
        {"interval",  config.interval},
        {"modcod",    config.modcod},
        {"data_size", config.data_size},
    };
}

inline void from_json(const nlohmann::json& json, Input::Config &config) {

    config = {
        
        .first_id  = json.at("first_id"),
        .interval  = json.at("interval"),
        .modcod    = json.at("modcod"),
        .data_size = json.at("data_size"),
    };
}


}  // namespace Components
