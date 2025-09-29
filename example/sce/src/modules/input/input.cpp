/**
 * \file
 * \brief Contains input class implementation.
 */

// ============================================================================

#include <cassert>

#include <modules/input/input.h>

#ifdef USE_QT_GUI

#include <QTextEdit>
#include "gui/qt/InputSettingsWidget/inputsettingswidget.h"

#endif

// ============================================================================

using namespace Components;

// ============================================================================

Input::Input(Config Config, IBlockCommunicator* next_hop) : config(Config), next_id_(config.first_id), next_hop_(next_hop) {
    assert(config.interval.asNanoseconds() > 0);
}

void Input::OnTimeStamp(MeasureUnits::Time) {
    if (time_left_.asNanoseconds() <= 0) {
        Packet new_packet(PacketHeader(next_id_++, config.modcod));

        new_packet.SetPayload(std::vector<Packet::DataType>(config.data_size, Packet::DataType(0)));

        Send(next_hop_, new_packet);

        time_left_ = config.interval;

    }

    time_left_ -= MeasureUnits::nanoseconds(1);
}

void Input::SetConfig(const nlohmann::json& cfg) {
    config.first_id = cfg["first_id"].get<PacketHeader::Id>();
    config.modcod = cfg["modcod"].get<ModCod::Type>();
    config.interval = cfg["interval"].get<MeasureUnits::Duration>();
    config.data_size = cfg["data_size"].get<size_t>();
}

nlohmann::json Input::GetConfig () const {
  nlohmann::json configJSON = {
      {"first_id", config.first_id},
      {"modcod", config.modcod},
      {"interval", config.interval},
      {"data_size", config.data_size},
  };

  return configJSON;
}

#ifdef USE_QT_GUI

AbstractSettingsWidget* Input::GetSettingsWidget() {
    AbstractSettingsWidget *widget = new InputSettingsWidget();
    widget->setObjectName("config");
    return widget;
}

QWidget* Input::GetStatsWidget() {
    QTextEdit* stat_widget = new QTextEdit();
    stat_widget->setReadOnly(true);
    stat_widget->setObjectName("statWidget");
    return stat_widget;
}

#endif

