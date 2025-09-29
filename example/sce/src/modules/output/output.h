/**
 * \file
 * \brief Contains output class declaration.
 */

#pragma once

// ============================================================================

#include <vector>

#include <interfaces.h>
#include <emulatedblock.h>
#include <packet.h>

#ifdef USE_QT_GUI

#include <qtinterfaces.h>

#endif

// ============================================================================

namespace Components {

  // Calculates some metrics from received packages.
class Output final :
#ifdef USE_QT_GUI
public IQTWidgetConfigurator,
#endif
public Component, public IReceiver, public IConfigurator {
  public:
    /// Some info about received packages.
    struct Metrics {
        size_t total_accepted = 0;  ///< Amount of packages received.
        size_t order_error = 0;     ///< Received some packages in incorrect order.
        size_t missing_error = 0;   ///< Some packages from id range are missing.
    };

    struct Config {
        PacketHeader::Id first_id;
        PacketHeader::Id last_id;

        bool operator== (const Config &other) const = default;
    };

    /// Creates output module.
    Output(Config Config);

    // Non-Copyable and Non-Movable
    // Output(const Output&) = delete;
    // Output& operator=(const Output&) = delete;

    /// Receives package.
    void OnReceive (Packet packet) override;

    /// Calculates metrics based on package history.
    Metrics GetMetrics();

    /// Clears package history.
    void Reset();

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
    
    std::vector<PacketHeader::Id> ids_;
};


//For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const Output::Config &config) {

    json = nlohmann::json {

        {"first_id", config.first_id},
        {"last_id",  config.last_id},
    };
}

inline void from_json(const nlohmann::json& json, Output::Config &config) {

    config = {
        
        .first_id = json.at("first_id"),
        .last_id  = json.at("last_id"),
    };
}


}   // namespace Components
