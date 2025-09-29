#include <modules/demodulator/demodulator.h>

#include <cmath>

#ifdef USE_QT_GUI
#include <QTextEdit>
#endif

namespace Components { 

const MeasureUnits::Frequency kDefaultFrequency = MeasureUnits::hertz(2100);
const MeasureUnits::Frequency kEps = MeasureUnits::hertz(10);

Demodulator::Demodulator(const Config& Config, IBlockCommunicator* nextHop) :
    config(Config), next_hop_(nextHop) {}

Demodulator::Demodulator(IBlockCommunicator* nextHop) :
    config(Config{kDefaultFrequency}), next_hop_(nextHop) {}

void Demodulator::OnReceive(Packet packet) {
    if (next_hop_ == nullptr) {
        return;
    }

    for (auto frequency : config.frequencies) {
        if (std::abs(packet.header.carrier_frequency - frequency) <= kEps.asHertz()) {
            Send(next_hop_, packet);
        }
    }
}

void Demodulator::SetConfig(const nlohmann::json& cfg) {
    config.frequencies = cfg["frequencies"].get<std::vector<MeasureUnits::Frequency>>();
}

nlohmann::json Demodulator::GetConfig () const {
    nlohmann::json configJSON = {
        {"frequencies", config.frequencies},
    };

    return configJSON;
}

#ifdef USE_QT_GUI
DefaultSettingsWidget* Demodulator::GetSettingsWidget() {
  DefaultSettingsWidget* widget = new DefaultSettingsWidget();
  widget->setObjectName("config");
  return widget;
}

QWidget* Demodulator::GetStatsWidget() {
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}
#endif


}