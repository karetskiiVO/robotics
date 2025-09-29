#include "inputsettingswidget.h"
#include "ui_inputsettingswidget.h"

InputSettingsWidget::InputSettingsWidget(QWidget *parent)
    : SettingsWidget(parent)
    , ui(new Ui::InputSettingsWidget)
{
    ui->setupUi(this);
    Init();
}

InputSettingsWidget::~InputSettingsWidget()
{
    delete ui;
}
