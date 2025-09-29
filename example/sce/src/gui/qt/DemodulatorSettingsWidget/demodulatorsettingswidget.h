#ifndef DEMODULATORSETTINGSWIDGET_H
#define DEMODULATORSETTINGSWIDGET_H

#include "abstractsettingswidget.h"
#include "nlohmann/json.hpp"

namespace Ui {
class DemodulatorSettingsWidget;
}

/**
 * @class DemodulatorSettingsWidget
 * @brief A widget to store settings of Demodulator block
 */
class DemodulatorSettingsWidget : public AbstractSettingsWidget {
  Q_OBJECT

protected:
  void Init() override;
public slots:
  void saveSettings() override;
  void restoreSettings() override;

public:
  explicit DemodulatorSettingsWidget(QWidget *parent = nullptr);
  ~DemodulatorSettingsWidget() override;
  std::string SettingsJSON() const override;

private:
  nlohmann::json jsonSettings;
  nlohmann::json oldJsonSettings;
  Ui::DemodulatorSettingsWidget *ui;
};

#endif // DEMODULATORSETTINGSWIDGET_H
