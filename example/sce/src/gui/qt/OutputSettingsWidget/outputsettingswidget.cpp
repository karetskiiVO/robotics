#include "outputsettingswidget.h"
#include "ui_outputsettingswidget.h"

OutputSettingsWidget::OutputSettingsWidget(QWidget *parent)
    : SettingsWidget(parent)
    , ui(new Ui::OutputSettingsWidget)
{
    ui->setupUi(this);
    ui->first_id->setMinimum(0);
    ui->last_id->setMinimum(0);
    Init();

}

OutputSettingsWidget::~OutputSettingsWidget()
{
    delete ui;
}
