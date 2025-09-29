#include "emulation_worker.h"

#include "baseblockwidget.h"

EmulationWorker::EmulationWorker(QObject* parent) : QThread(parent) {
  IQTWidgetConfigurator* input =
      line_.GetBlock(EmulationLine::ConfigTag::InputTag)->GetComponent<Input>();
  configurators_map_["input"] = input;
  settings_widgets_map_["input"] = input->GetSettingsWidget();
  stats_widgets_map_["input"] =
      qobject_cast<QTextEdit*>(input->GetStatsWidget());

  IQTWidgetConfigurator* modulator =
      line_.GetBlock(EmulationLine::ConfigTag::ModulatorTag)
          ->GetComponent<Modulator>();
  configurators_map_["modulator"] = modulator;
  settings_widgets_map_["modulator"] = modulator->GetSettingsWidget();

  stats_widgets_map_["modulator"] =
      qobject_cast<QTextEdit*>(modulator->GetStatsWidget());

  IQTWidgetConfigurator* buc =
      line_.GetBlock(EmulationLine::ConfigTag::BUCTag)->GetComponent<BUC>();
  configurators_map_["buc"] = buc;
  settings_widgets_map_["buc"] = buc->GetSettingsWidget();
  stats_widgets_map_["buc"] = qobject_cast<QTextEdit*>(buc->GetStatsWidget());

  IQTWidgetConfigurator* uplink =
      line_.GetBlock(EmulationLine::ConfigTag::LineUpTag)->GetComponent<Line>();

  configurators_map_["uplink"] = uplink;
  settings_widgets_map_["uplink"] = uplink->GetSettingsWidget();
  stats_widgets_map_["uplink"] =
      qobject_cast<QTextEdit*>(uplink->GetStatsWidget());

  IQTWidgetConfigurator* downlink =
      line_.GetBlock(EmulationLine::ConfigTag::LineDownTag)
          ->GetComponent<Line>();
  configurators_map_["downlink"] = downlink;
  settings_widgets_map_["downlink"] = downlink->GetSettingsWidget();
  stats_widgets_map_["downlink"] =
      qobject_cast<QTextEdit*>(downlink->GetStatsWidget());

  IQTWidgetConfigurator* lnb =
      line_.GetBlock(EmulationLine::ConfigTag::LNBTag)->GetComponent<LNB>();
  configurators_map_["lnb"] = lnb;
  settings_widgets_map_["lnb"] = lnb->GetSettingsWidget();
  stats_widgets_map_["lnb"] = qobject_cast<QTextEdit*>(lnb->GetStatsWidget());

  IQTWidgetConfigurator* demodulator =
      line_.GetBlock(EmulationLine::ConfigTag::DemodulatorTag)
          ->GetComponent<Demodulator>();

  configurators_map_["demodulator"] = demodulator;
  settings_widgets_map_["demodulator"] = demodulator->GetSettingsWidget();
  stats_widgets_map_["demodulator"] =
      qobject_cast<QTextEdit*>(demodulator->GetStatsWidget());

  IQTWidgetConfigurator* output =
      line_.GetBlock(EmulationLine::ConfigTag::OutputTag)
          ->GetComponent<Output>();
  configurators_map_["output"] = output;
  settings_widgets_map_["output"] = output->GetSettingsWidget();
  stats_widgets_map_["output"] =
      qobject_cast<QTextEdit*>(output->GetStatsWidget());
}

void EmulationWorker::FillWidgets(BaseBlockWidget* base_block,
                                  const QString& configurator_name) {

  base_block->SetStatsWidget(stats_widgets_map_[configurator_name]);
  base_block->SetSettingsWidget(settings_widgets_map_[configurator_name]);
}

void EmulationWorker::SetRelatedBlocks(BlocksAreaWidget* blocks_area) {
  if (blocks_) {
    const QPushButton* button = blocks_->CornerButton();
    disconnect(button, &QPushButton::clicked, this, nullptr);
  }

  blocks_ = blocks_area;

  if (!blocks_) {
    return;
  }

  const int tsbs_count = blocks_->count();
  for (int i = 0; i < tsbs_count; ++i) {
    if (QWidget* current = blocks_->widget(i);
        current && configurators_map_.contains(current->objectName())) {
      auto base_block = qobject_cast<BaseBlockWidget*>(current);
      FillWidgets(base_block, current->objectName());
    }
  }

  const QPushButton* button = blocks_->CornerButton();
  connect(button, &QPushButton::clicked, this, [button, this]() {
    if (!button->isChecked()) {
      this->line_.Stop();
      this->quit();
      return;
    }
    this->start();
  });
}

void EmulationWorker::run() {
  if (!blocks_) {
    throw std::runtime_error("Cannot initialize blocks, because they are null");
  }
  for (auto& [name, settings] : settings_widgets_map_) {
    if (auto* configurable =
            dynamic_cast<IConfigurator*>(configurators_map_[name]);
        configurable) {
      configurable->SetConfig(settings->SettingsJSON());
    }
  }
  line_.Emulate();
}
