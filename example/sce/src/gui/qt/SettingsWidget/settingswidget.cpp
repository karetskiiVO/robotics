#include "settingswidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QJsonDocument>
#include <QSpinBox>
#include <qlongspinbox.h>

SettingsWidget::SettingsWidget(QWidget *parent)
    : AbstractSettingsWidget(parent)
{}

std::string SettingsWidget::SettingsJSON() const
{
    return nlohmann::to_string(oldJsonSettings);
}

void SettingsWidget::onSettingsChanged()
{
    emit settingsChanged();
}

void SettingsWidget::Init()
{
    for (const QCheckBox *checkBox :
         static_cast<const QList<QCheckBox *>>(this->findChildren<QCheckBox *>())) {
        jsonSettings[checkBox->objectName().toStdString()] = checkBox->isChecked();
        connect(
            checkBox,
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
            &QCheckBox::checkStateChanged,
#else
            &QCheckBox::stateChanged,
#endif
            this,
            &SettingsWidget::onSettingsChanged);
    }

    for (const QSpinBox *spinBox :
         static_cast<const QList<QSpinBox *>>(this->findChildren<QSpinBox *>())) {
        jsonSettings[spinBox->objectName().toStdString()] = spinBox->value();
        connect(
            spinBox,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &SettingsWidget::onSettingsChanged);
    }

    for (const QDoubleSpinBox *spinBox :
         static_cast<const QList<QDoubleSpinBox *>>(this->findChildren<QDoubleSpinBox *>())) {
        jsonSettings[spinBox->objectName().toStdString()] = spinBox->value();
        connect(
            spinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            &SettingsWidget::onSettingsChanged);
    }

    for (const QComboBox *comboBox :
         static_cast<const QList<QComboBox *>>(this->findChildren<QComboBox *>())) {
        jsonSettings[comboBox->objectName().toStdString()] = comboBox->currentIndex();
        connect(comboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onSettingsChanged);
    }

    for (const QLongSpinBox *spinBox :
         static_cast<const QList<QLongSpinBox *>>(this->findChildren<QLongSpinBox *>())) {
        jsonSettings[spinBox->objectName().toStdString()] = spinBox->value();
        connect(
            spinBox,
            QOverload<qlonglong>::of(&QLongSpinBox::valueChanged),
            this,
            &SettingsWidget::onSettingsChanged);
    }
    oldJsonSettings = jsonSettings;
}

void SettingsWidget::saveSettings()
{
    for (const QCheckBox *checkBox :
         static_cast<const QList<QCheckBox *>>(this->findChildren<QCheckBox *>())) {
        jsonSettings[checkBox->objectName().toStdString()] = checkBox->isChecked();
    }

    for (const QSpinBox *spinBox :
         static_cast<const QList<QSpinBox *>>(this->findChildren<QSpinBox *>())) {
        jsonSettings[spinBox->objectName().toStdString()] = spinBox->value();
    }

    for (const QDoubleSpinBox *spinBox :
         static_cast<const QList<QDoubleSpinBox *>>(this->findChildren<QDoubleSpinBox *>())) {
        jsonSettings[spinBox->objectName().toStdString()] = spinBox->value();
    }

    for (const QComboBox *comboBox :
         static_cast<const QList<QComboBox *>>(this->findChildren<QComboBox *>())) {
        jsonSettings[comboBox->objectName().toStdString()] = comboBox->currentIndex();
    }

    for (const QLongSpinBox *spinBox :
         static_cast<const QList<QLongSpinBox *>>(this->findChildren<QLongSpinBox *>())) {
        jsonSettings[spinBox->objectName().toStdString()] = spinBox->value();
    }
    oldJsonSettings = jsonSettings;
}

void SettingsWidget::restoreSettings()
{
    jsonSettings = oldJsonSettings;
    for (QCheckBox *checkBox : this->findChildren<QCheckBox *>()) {
        checkBox->setChecked(jsonSettings[checkBox->objectName().toStdString()].get<bool>());
    }

    for (QSpinBox *spinBox : this->findChildren<QSpinBox *>()) {
        spinBox->setValue(jsonSettings[spinBox->objectName().toStdString()].get<int>());
    }

    for (QDoubleSpinBox *doubleSpinBox : this->findChildren<QDoubleSpinBox *>()) {
        doubleSpinBox->setValue(jsonSettings[doubleSpinBox->objectName().toStdString()].get<double>());
    }

    for (QComboBox *comboBox : this->findChildren<QComboBox *>()) {
        comboBox->setCurrentIndex(jsonSettings[comboBox->objectName().toStdString()].get<int>());
    }

    for (QLongSpinBox *spinBox : this->findChildren<QLongSpinBox *>()) {
        spinBox->setValue(jsonSettings[spinBox->objectName().toStdString()].get<qlonglong>());
    }
}
