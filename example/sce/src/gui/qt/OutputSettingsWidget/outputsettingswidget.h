#ifndef OUTPUTSETTINGSWIDGET_H
#define OUTPUTSETTINGSWIDGET_H

#include <gui/qt/SettingsWidget/settingswidget.h>

namespace Ui {
class OutputSettingsWidget;
}

class OutputSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit OutputSettingsWidget(QWidget *parent = nullptr);
    ~OutputSettingsWidget();

private:
    Ui::OutputSettingsWidget *ui;
};

#endif // OUTPUTSETTINGSWIDGET_H
