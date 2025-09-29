#pragma once

#include <stdexcept>

#ifdef USE_QT_GUI
#include <qtinterfaces.h>
#endif


#include <interfaces.h>
#include <emulatedblock.h>

#include <utils/MeasureUnits/MeasureUnits.h>


namespace Components {

class Modulator : 
#ifdef USE_QT_GUI
    public IQTWidgetConfigurator,
#endif
    public Component, public IReceiver, public IConfigurator {
public:

    struct Config {

        MeasureUnits::Frequency carrier_frequency;  
        MeasureUnits::Frequency clock_frequency;   

        bool operator==(const Config&) const = default;
    };

    Modulator() = delete;

    explicit Modulator(const Config& config, IBlockCommunicator* next_hop);
    
    explicit Modulator(IBlockCommunicator* next_hop);

    ~Modulator() = default;

    void OnReceive(Packet packet);

    void SetConfig(const nlohmann::json&);

    nlohmann::json GetConfig () const;

#ifdef USE_QT_GUI
    AbstractSettingsWidget* GetSettingsWidget() override;

    QWidget* GetStatsWidget() override;
#endif

    Config config;

private:

    IBlockCommunicator* next_hop_;
};


//For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const Modulator::Config &config) {

    json["carrier_frequency"] = config.carrier_frequency;
    json["clock_frequency"]   = config.clock_frequency;
}

inline void from_json(const nlohmann::json& json, Modulator::Config &config) {

    config = {
        
        .carrier_frequency = json["carrier_frequency"],
        .clock_frequency   = json["clock_frequency"],
    };
}


}