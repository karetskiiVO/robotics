#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <nlohmann/json.hpp>

#include "abstractsettingswidget.h"

class SettingsWidget : public AbstractSettingsWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    std::string SettingsJSON() const override;

protected:
    void Init() final;

public slots:
    void saveSettings() final;
    void restoreSettings() final;

private:
    nlohmann::json jsonSettings;
    nlohmann::json oldJsonSettings;

private slots:
    void onSettingsChanged();
};

#endif // SETTINGWIDGET_H
