#pragma once


#include <interfaces.h>
#include <emulatedblock.h>

#include <utils/MeasureUnits/MeasureUnits.h>

#include <map>

#ifdef USE_QT_GUI

#include <qtinterfaces.h>

#endif


namespace Components {


class Satellite:
#ifdef USE_QT_GUI
public IQTWidgetConfigurator,
#endif
public Component, public IReceiver, public IConfigurator {
public:
    struct Config {     //maybe change the config
        Gain gain;
        MeasureUnits::Frequency transferFrequency;

        bool operator== (const Config &other) const = default;
    };

    Satellite (Config config, const std::vector<IBlockCommunicator*>& nextHop = std::vector<IBlockCommunicator*>());

    Satellite (const Satellite&) = default;
    Satellite& operator= (const Satellite& other) = default;

    ~Satellite () = default;

    //JSON config
    void SetConfig (const nlohmann::json& config) override;
    nlohmann::json GetConfig () const override;

    //Widgets
    #ifdef USE_QT_GUI

    AbstractSettingsWidget* GetSettingsWidget() override;
    QWidget* GetStatsWidget() override;

    #endif

    void OnReceive (Packet packet) override;
    
    void RegisterNewHop (IBlockCommunicator* newHop) { nextHops_.push_back(newHop); }

  private:
    Config config_;
    std::vector<IBlockCommunicator*> nextHops_;
};


//For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const Satellite::Config &config) {

    json = nlohmann::json {

        {"gain",              config.gain},
        {"transferFrequency", config.transferFrequency},
    };
}

inline void from_json(const nlohmann::json& json, Satellite::Config &config) {

    config = {
        
        .gain              = json.at("gain"),
        .transferFrequency = json.at("transferFrequency"),
    };
}


}