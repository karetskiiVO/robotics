#pragma once

#include <stdexcept>

#ifdef USE_QT_GUI
#include <qtinterfaces.h>
#endif

#include <interfaces.h>
#include <emulatedblock.h>

namespace Components {

class Demodulator : 
#ifdef USE_QT_GUI
    public IQTWidgetConfigurator,
#endif
    public Component, public IReceiver, public IConfigurator    {
public:
    
    struct Config {
        std::vector<MeasureUnits::Frequency> frequencies;

        Config (): frequencies() {}
        Config(MeasureUnits::Frequency frequency) : frequencies{frequency} {}

        bool operator==(const Config&) const = default;
    };

    Demodulator() = delete;

    explicit Demodulator(IBlockCommunicator* nextHop);

    explicit Demodulator(const Config& Config, IBlockCommunicator* nextHop);

    ~Demodulator() = default;

    void OnReceive(Packet packet);

    void SetConfig(const nlohmann::json&);

    nlohmann::json GetConfig () const;

#ifdef USE_QT_GUI
    DefaultSettingsWidget* GetSettingsWidget() override;

    QWidget* GetStatsWidget() override;
#endif

    Config config;

private:

    IBlockCommunicator* next_hop_;
};


//For nlohmann::json serialization
inline void to_json(nlohmann::json& json, const Demodulator::Config &config) {

    json = nlohmann::json {
        
        {"frequencies", config.frequencies}
    };
}

inline void from_json(const nlohmann::json& json, Demodulator::Config &config) {

    config.frequencies = json.at("frequencies").get<std::vector<MeasureUnits::Frequency>>();
}


}