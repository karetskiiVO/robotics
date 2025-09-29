#include "linesettingswidget.h"
#include "ui_linesettingswidget.h"

LineSettingsWidget::LineSettingsWidget(QWidget *parent)
    : SettingsWidget(parent)
    , ui(new Ui::LineSettingsWidget)
{
    ui->setupUi(this);
    ui->attenuation->setMaximum(std::numeric_limits<double>::max());
    Init();
}

LineSettingsWidget::~LineSettingsWidget()
{
    delete ui;
}
