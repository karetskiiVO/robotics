#include "bucsettingswidget.h"
#include "ui_bucsettingswidget.h"

BUCSettingsWidget::BUCSettingsWidget(QWidget *parent)
    : SettingsWidget(parent)
    , ui(new Ui::BUCSettingsWidget)
{
    ui->setupUi(this);
    ui->transferFrequency->setMinimum(std::max(0LL, std::numeric_limits<decltype(ui->transferFrequency->value())>::min()));
    ui->gain->setMinimum(std::max(0LL, std::numeric_limits<decltype(ui->gain->value())>::min()));
    ui->maxPower->setMinimum(std::max(0LL, std::numeric_limits<decltype(ui->maxPower->value())>::min()));
    Init();
}

BUCSettingsWidget::~BUCSettingsWidget()
{
    delete ui;
}
