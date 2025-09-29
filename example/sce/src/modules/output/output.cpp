/**
 * \file
 * \brief Contains output class implementation.
 */

// ============================================================================

#include "output.h"

#ifdef USE_QT_GUI

#include <QTextEdit>
#include <gui/qt/OutputSettingsWidget/outputsettingswidget.h>
#endif

// ============================================================================

using namespace Components;

// ============================================================================

Output::Output(Config Config)
    : config(Config) {}

void Output::OnReceive(Packet packet) {
    // Ignore
    if (packet.header.id < config.first_id || packet.header.id > config.last_id) {
        return;
    }

    // Accept
    ids_.push_back(packet.header.id);
}

Output::Metrics Output::GetMetrics() {
    Metrics metrics = {};

    metrics.total_accepted = ids_.size();

    for (size_t i = 1; i < ids_.size(); i++) {
        if (ids_[i] < ids_[i - 1]) {
            metrics.order_error++;
        }
    }

    metrics.missing_error = config.last_id - config.first_id + 1 - ids_.size();

    return metrics;
}

void Output::Reset() { ids_.clear(); }

void Output::SetConfig(const nlohmann::json& cfg) {
    config.first_id = cfg["first_id"].get<PacketHeader::Id>();
    config.last_id = cfg["last_id"].get<PacketHeader::Id>();
}

nlohmann::json Output::GetConfig () const {
    nlohmann::json cfg = {
        {"first_id", config.first_id},
        {"last_id", config.last_id},
    };

    return cfg;
}

#ifdef USE_QT_GUI

AbstractSettingsWidget* Output::GetSettingsWidget() {
  AbstractSettingsWidget* widget = new OutputSettingsWidget();
  widget->setObjectName("config");
  return widget;
}

QWidget* Output::GetStatsWidget() {
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}

#endif
