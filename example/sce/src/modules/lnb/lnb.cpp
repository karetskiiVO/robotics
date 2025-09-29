#include <modules/lnb/lnb.h>

#ifdef USE_QT_GUI
#include <QTextEdit>
#include <gui/qt/BUCSettingsWidget/bucsettingswidget.h>
#endif

using namespace Components;

const uint64_t kGHzInHz = 1'000'000'000;

const MeasureUnits::Frequency kDefaultFrequency = MeasureUnits::hertz(13'000'000'000);
const MeasureUnits::Power kDefaultGain  = MeasureUnits::watt(25.0);
const MeasureUnits::Power kDefaultPower = MeasureUnits::watt(100.0);

LNB::LNB (const Config& Config, IBlockCommunicator* nextHop) : 
    config(Config), next_hop_(nextHop) {}

LNB::LNB (IBlockCommunicator* nextHop) : 
    config(Config{kDefaultGain, kDefaultPower, kDefaultFrequency}), next_hop_(nextHop) {}

void LNB::OnReceive (Packet packet) {
    if (next_hop_ == nullptr) return;
    
    packet.header.carrier_frequency -= config.transferFrequency;
    packet.header.power = std::max(packet.header.power - config.gain, config.minPower);  

    Send(next_hop_, packet);
}

void LNB::SetConfig(const nlohmann::json& cfg) {
    config.gain = cfg["gain"].get<MeasureUnits::Power>();
    config.minPower = cfg["minPower"].get<MeasureUnits::Power>();
    config.transferFrequency = cfg["transferFrequency"].get<MeasureUnits::Frequency>();
}

nlohmann::json LNB::GetConfig () const {
    nlohmann::json configJSON = {
        {"gain", config.gain},
        {"minPower", config.minPower},
        {"transferFrequency", config.transferFrequency}
    };

    return configJSON;
}

#ifdef USE_QT_GUI
AbstractSettingsWidget* LNB::GetSettingsWidget() {
  AbstractSettingsWidget* widget = new BUCSettingsWidget();
  widget->setObjectName("config");
  return widget;
}

QWidget* LNB::GetStatsWidget() {
    // TODO: Maybe something more complicated than QTextEdit is needed for StatWidget.
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}
#endif
