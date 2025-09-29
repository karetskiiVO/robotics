#pragma once

#ifndef LNB_h
#define LNB_h

#ifdef USE_QT_GUI
#include <qtinterfaces.h>
#endif

#include <interfaces.h>
#include <emulatedblock.h>

#include <utils/MeasureUnits/MeasureUnits.h>


namespace Components {

class LNB :
#ifdef USE_QT_GUI
    public IQTWidgetConfigurator,
#endif
    public Component, public IReceiver, public IConfigurator {
public:
    struct Config {
        MeasureUnits::Power gain;
        MeasureUnits::Power minPower;
        MeasureUnits::Frequency transferFrequency;

        bool operator== (const Config &other) const = default;
    };

   explicit LNB(const Config& Config, IBlockCommunicator* nextHop);

   explicit LNB(IBlockCommunicator* nextHop);

    void OnReceive(Packet packet);

    void SetConfig(const nlohmann::json&);

    nlohmann::json GetConfig() const;
 
#ifdef USE_QT_GUI
    AbstractSettingsWidget* GetSettingsWidget() override;

    QWidget* GetStatsWidget() override;
#endif

public:

    Config config;

private:
    
    IBlockCommunicator* next_hop_ = nullptr;
};


//For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const LNB::Config &config) {

    json = nlohmann::json {

        {"gain",              config.gain},
        {"minPower",          config.minPower},
        {"transferFrequency", config.transferFrequency},
    };
}

inline void from_json(const nlohmann::json& json, LNB::Config &config) {

    config = {
        
        .gain              = json.at("gain"),
        .minPower          = json.at("minPower"),
        .transferFrequency = json.at("transferFrequency"),
    };
}


}

#endif // lnb.h
