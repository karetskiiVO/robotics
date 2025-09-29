#ifndef MODULATORSETTINGSWIDGET_H
#define MODULATORSETTINGSWIDGET_H

#include <gui/qt/SettingsWidget/settingswidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class ModulatorSettingsWidget;
}
QT_END_NAMESPACE

/**
 * @class ModulatorSettingsWidget
 * @brief A widget to store settings of Modulator block
 */
class ModulatorSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit ModulatorSettingsWidget(QWidget *parent = nullptr);
    ~ModulatorSettingsWidget();

private:
    Ui::ModulatorSettingsWidget *ui;
};

#endif // MODULATORSETTINGSWIDGET_H
