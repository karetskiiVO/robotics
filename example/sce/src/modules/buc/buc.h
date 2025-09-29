#pragma once

#ifndef BUC_h
#define BUC_h

#ifdef USE_QT_GUI
#include <qtinterfaces.h>
#endif

#include <interfaces.h>
#include <emulatedblock.h>

namespace Components {


class BUC :
#ifdef USE_QT_GUI
    public IQTWidgetConfigurator,
#endif
    public Component, public IReceiver, public IConfigurator {
public:
    struct Config {
        MeasureUnits::Power gain;
        MeasureUnits::Power maxPower;
        MeasureUnits::Frequency transferFrequency;

        bool operator== (const Config &other) const = default;
    };

    explicit BUC(const Config& config, IBlockCommunicator* nextHop);
    
    explicit BUC(IBlockCommunicator* nextHop);

    void OnReceive(Packet packet);

    void SetConfig(const nlohmann::json&);

    nlohmann::json GetConfig () const;

#ifdef USE_QT_GUI
    AbstractSettingsWidget* GetSettingsWidget() override;

    QWidget* GetStatsWidget() override;
#endif

private:
    Config config_;
    IBlockCommunicator* next_hop_ = nullptr;
};


//For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const BUC::Config &config) {

    json = nlohmann::json {
        
        {"gain",              config.gain},
        {"maxPower",          config.maxPower},
        {"transferFrequency", config.transferFrequency},
    };
}

inline void from_json(const nlohmann::json& json, BUC::Config &config) {

    config = {
        
        .gain              = json.at("gain"),
        .maxPower          = json.at("maxPower"),
        .transferFrequency = json.at("transferFrequency"),
    };
}


}

#endif // buc.h
