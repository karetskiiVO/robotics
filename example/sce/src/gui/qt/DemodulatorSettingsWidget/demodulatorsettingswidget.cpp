#include "demodulatorsettingswidget.h"
#include "ui_demodulatorsettingswidget.h"

#include <QLineEdit>

static nlohmann::json QStringToJsonArray(const QString &str) {
  nlohmann::json result = nlohmann::json::array();
  for (const auto &value : str.split(',', Qt::SkipEmptyParts)) {
    bool ok = false;
    auto intVal = value.trimmed().toLong(&ok);
    if (ok) {
      result.emplace_back(intVal);
    }
  }
  return result;
}

static QString JsonArrayToQString(const nlohmann::json &json) {
  const auto str = nlohmann::to_string(json);
  if (str.front() == '[' && str.back() == ']') {
    return QString::fromStdString(str.substr(1, str.size() - 2));
  }
  return QString::fromStdString(str);
}

void DemodulatorSettingsWidget::Init() {
  for (const QLineEdit *textEdit : static_cast<const QList<QLineEdit *>>(
           this->findChildren<QLineEdit *>())) {
    // Convert text to json array
    jsonSettings[textEdit->objectName().toStdString()] =
        QStringToJsonArray(textEdit->text());
    connect(textEdit, &QLineEdit::textEdited, this,
            [this]() { emit settingsChanged(); });
  }
  oldJsonSettings = jsonSettings;
}

DemodulatorSettingsWidget::DemodulatorSettingsWidget(QWidget *parent)
    : AbstractSettingsWidget(parent), ui(new Ui::DemodulatorSettingsWidget) {
  ui->setupUi(this);
  DemodulatorSettingsWidget::Init();
}
DemodulatorSettingsWidget::~DemodulatorSettingsWidget() { delete ui; }

void DemodulatorSettingsWidget::saveSettings() {
  for (const QLineEdit *textEdit : static_cast<const QList<QLineEdit *>>(
           this->findChildren<QLineEdit *>())) {
    jsonSettings[textEdit->objectName().toStdString()] =
        QStringToJsonArray(textEdit->text());
  }
}

void DemodulatorSettingsWidget::restoreSettings() {
  for (QLineEdit *textEdit : this->findChildren<QLineEdit *>()) {
    textEdit->setText(
        JsonArrayToQString(jsonSettings[textEdit->objectName().toStdString()]));
  }
}

std::string DemodulatorSettingsWidget::SettingsJSON() const {
  return nlohmann::to_string(oldJsonSettings);
}
