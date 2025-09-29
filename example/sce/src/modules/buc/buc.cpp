#include <modules/buc/buc.h>

#ifdef USE_QT_GUI
#include <QTextEdit>
#include <gui/qt/BUCSettingsWidget/bucsettingswidget.h>
#endif

using namespace Components;

const uint64_t kGHzInHz = 1'000'000'000;

const MeasureUnits::Frequency kDefaultFrequency = MeasureUnits::hertz(13'000'000'000);
const MeasureUnits::Power kDefaultGain  = MeasureUnits::watt(25.0);
const MeasureUnits::Power kDefaultPower = MeasureUnits::watt(100.0);

BUC::BUC (const Config& config, IBlockCommunicator* nextHop) : 
    config_(config), next_hop_(nextHop) {}

BUC::BUC (IBlockCommunicator* nextHop) : 
config_(Config{kDefaultGain, kDefaultPower, kDefaultFrequency}), next_hop_(nextHop) {}

void BUC::OnReceive (Packet packet) {
    if (next_hop_ == nullptr) return;

    packet.header.carrier_frequency += config_.transferFrequency;
    packet.header.power = std::min(packet.header.power + config_.gain, config_.maxPower);   

    Send(next_hop_, packet);
}

void BUC::SetConfig(const nlohmann::json& cfg) {
    config_.gain = cfg["gain"].get<MeasureUnits::Power>();
    config_.maxPower = cfg["maxPower"].get<MeasureUnits::Power>();
    config_.transferFrequency = cfg["transferFrequency"].get<MeasureUnits::Frequency>();
}

nlohmann::json BUC::GetConfig () const {
    nlohmann::json config = {
        {"gain", config_.gain},
        {"maxPower", config_.maxPower},
        {"transferFrequency", config_.transferFrequency}
    };

    return config;
}

#ifdef USE_QT_GUI
AbstractSettingsWidget* BUC::GetSettingsWidget() {
  AbstractSettingsWidget* widget = new BUCSettingsWidget();
  widget->setObjectName("config");
  return widget;
}

QWidget* BUC::GetStatsWidget() {
    // TODO: Maybe something more complicated than QTextEdit is needed for StatWidget.
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}
#endif
