#ifndef SATELLITESETTINGSWIDGET_H
#define SATELLITESETTINGSWIDGET_H

#include <gui/qt/SettingsWidget/settingswidget.h>

namespace Ui {
class SatelliteSettingsWidget;
}

class SatelliteSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit SatelliteSettingsWidget(QWidget *parent = nullptr);
    ~SatelliteSettingsWidget();

private:
    Ui::SatelliteSettingsWidget *ui;
};

#endif // SATELLITESETTINGSWIDGET_H
