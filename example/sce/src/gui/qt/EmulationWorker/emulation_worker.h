#ifndef SCE_EMULATION_WORKER_H
#define SCE_EMULATION_WORKER_H
#include <QThread>
#include <QTextEdit>
#include "baseblockwidget.h"
#include "blocksareawidget.h"

#include "emulatedline.h"

class EmulationWorker final : public QThread {
  Q_OBJECT

 public:
  explicit EmulationWorker(QObject* parent = nullptr);

  void SetRelatedBlocks(BlocksAreaWidget* blocks_area);

 protected:
  void run() override;

 private:
  void FillWidgets(BaseBlockWidget* base_block,
                  const QString& configurator_name);

  EmulationLine line_;
  std::unordered_map<QString, QTextEdit*> stats_widgets_map_;
  std::unordered_map<QString, IQTWidgetConfigurator*> configurators_map_;
  std::unordered_map<QString, AbstractSettingsWidget*> settings_widgets_map_;
  BlocksAreaWidget* blocks_{nullptr};
};

#endif
