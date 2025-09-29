#ifndef BUCSETTINGSWIDGET_H
#define BUCSETTINGSWIDGET_H

#include <gui/qt/SettingsWidget/settingswidget.h>

namespace Ui {
class BUCSettingsWidget;
}

class BUCSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit BUCSettingsWidget(QWidget *parent = nullptr);
    ~BUCSettingsWidget();

private:
    Ui::BUCSettingsWidget *ui;
};

#endif // BUCSETTINGSWIDGET_H
