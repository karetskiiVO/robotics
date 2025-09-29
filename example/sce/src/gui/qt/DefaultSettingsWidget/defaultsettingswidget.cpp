#include "defaultsettingswidget.h"
#include "ui_defaultsettingswidget.h"

DefaultSettingsWidget::DefaultSettingsWidget(QWidget* parent)
    : AbstractSettingsWidget(parent), ui_(new Ui::DefaultSettingsWidget) {
    ui_->setupUi(this);
    connect(ui_->jsonConfigTextEdit, &QPlainTextEdit::textChanged, this,
            &DefaultSettingsWidget::settingsChanged);
}

DefaultSettingsWidget::~DefaultSettingsWidget() {
    delete ui_;
}

void DefaultSettingsWidget::Init() {}

std::string DefaultSettingsWidget::SettingsJSON() const {
  return old_.toStdString();
}

void DefaultSettingsWidget::saveSettings() {
    old_ = ui_->jsonConfigTextEdit->toPlainText();
}

void DefaultSettingsWidget::restoreSettings() {
    ui_->jsonConfigTextEdit->blockSignals(true);
    ui_->jsonConfigTextEdit->setPlainText(old_);
    ui_->jsonConfigTextEdit->blockSignals(false);
}