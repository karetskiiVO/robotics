#include "satellite.h"

#ifdef USE_QT_GUI

#include <QTextEdit>
#include <gui/qt/SatelliteSettingsWidget/satellitesettingswidget.h>
#endif

using namespace Components;


Satellite::Satellite (Config config, const std::vector<IBlockCommunicator*>& nextHops) : 
    config_(config), nextHops_(nextHops) {}

void Satellite::SetConfig(const nlohmann::json& config) {
    config_.gain              = config["gain"].get<Gain>();
    config_.transferFrequency = config["transferFrequency"].get<MeasureUnits::Frequency>();
}

nlohmann::json Satellite::GetConfig () const {

    nlohmann::json config = {
        {"gain",              config_.gain},
        {"transferFrequency", config_.transferFrequency}
    };

    return config;
}


#ifdef USE_QT_GUI

AbstractSettingsWidget* Satellite::GetSettingsWidget() {

    AbstractSettingsWidget* widget = new SatelliteSettingsWidget();
    widget->setObjectName("config");

    return widget;
}

QWidget* Satellite::GetStatsWidget() {

    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");

    return stat_widget;
}

#endif

void Satellite::OnReceive (Packet packet) {
    // packet.header.carrier_frequency = config_.transferFrequency;

    for (auto next_hop : nextHops_) {
        Send(next_hop, packet);
    }
}
