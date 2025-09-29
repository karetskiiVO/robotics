#include "satellitesettingswidget.h"
#include "ui_satellitesettingswidget.h"

SatelliteSettingsWidget::SatelliteSettingsWidget(QWidget *parent)
    : SettingsWidget(parent)
    , ui(new Ui::SatelliteSettingsWidget)
{
    ui->setupUi(this);
    ui->transferFrequency->setMinimum(std::max(0LL, std::numeric_limits<decltype(ui->transferFrequency->value())>::min()));
    ui->gain->setMinimum(std::max(0LL, std::numeric_limits<decltype(ui->gain->value())>::min()));
    Init();
}

SatelliteSettingsWidget::~SatelliteSettingsWidget()
{
    delete ui;
}
