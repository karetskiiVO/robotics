#include <modules/modulator/modulator.h>

#ifdef USE_QT_GUI
#include <QTextEdit>
#include <gui/qt/ModulatorSettingsWidget/modulatorsettingswidget.h>
#endif

namespace Components { 
    
const uint64_t kSecInNs = 1'000'000'000;

const MeasureUnits::Frequency kDefaultCarryFrequency = MeasureUnits::hertz(15 * 100'000'000);
const MeasureUnits::Frequency kDefaultClockFrequency = MeasureUnits::hertz(100);;

Modulator::Modulator(const Config& Config, IBlockCommunicator* next_hop):
    config(Config), next_hop_(next_hop) {}

Modulator::Modulator(IBlockCommunicator* next_hop):
    config(Config{kDefaultCarryFrequency, kDefaultClockFrequency}), next_hop_(next_hop) {}

void Modulator::OnReceive(Packet packet) {
    if (next_hop_ == nullptr) {
        return;
    }
    
    Send(next_hop_, packet);
}

void Modulator::SetConfig(const nlohmann::json& cfg) {

    config.carrier_frequency = cfg["carrier_frequency"].get<MeasureUnits::Frequency>();
    config.clock_frequency   = cfg["clock_frequency"].get<MeasureUnits::Frequency>();
}

nlohmann::json Modulator::GetConfig () const {

    nlohmann::json configJSON = {};
    configJSON["carrier_frequency"] = config.carrier_frequency;  
    configJSON["clock_frequency"]   = config.clock_frequency;

    return config;
}

#ifdef USE_QT_GUI
AbstractSettingsWidget* Modulator::GetSettingsWidget() {
  AbstractSettingsWidget* widget = new ModulatorSettingsWidget();
  widget->setObjectName("config");
  return widget;
}

QWidget* Modulator::GetStatsWidget() {
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}
#endif


}
