#ifndef INPUTSETTINGSWIDGET_H
#define INPUTSETTINGSWIDGET_H

#include <gui/qt/SettingsWidget/settingswidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class InputSettingsWidget;
}
QT_END_NAMESPACE

/**
 * @class InputSettingsWidget
 * @brief A widget to store settings of Input block
 */
class InputSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit InputSettingsWidget(QWidget *parent = nullptr);
    ~InputSettingsWidget() override;

private:
    Ui::InputSettingsWidget *ui;
};

#endif // INPUTSETTINGSWIDGET_H
