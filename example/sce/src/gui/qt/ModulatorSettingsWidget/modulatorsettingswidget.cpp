#include "modulatorsettingswidget.h"
#include "ui_modulatorsettingswidget.h"

ModulatorSettingsWidget::ModulatorSettingsWidget(QWidget *parent)
    : SettingsWidget(parent)
    , ui(new Ui::ModulatorSettingsWidget)
{
    ui->setupUi(this);
    ui->carrier_frequency->setMinimum(std::max(0LL, std::numeric_limits<decltype(ui->carrier_frequency->value())>::min()));
    ui->clock_frequency->setMinimum(std::max(0LL, std::numeric_limits<decltype(ui->clock_frequency->value())>::min()));
    Init();
}

ModulatorSettingsWidget::~ModulatorSettingsWidget()
{
    delete ui;
}
