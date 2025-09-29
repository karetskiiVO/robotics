#pragma once

#ifndef QTINTRFACES_h
#define QTINTRFACES_h

#include <QWidget>
// TODO: ugly include (maybe restructuring of code is needed)
#include <gui/qt/SettingsWidget/settingswidget.h>
#include <gui/qt/DefaultSettingsWidget/defaultsettingswidget.h>

class IQTWidgetConfigurator {
public:
    virtual AbstractSettingsWidget* GetSettingsWidget() = 0;
    
    virtual QWidget* GetStatsWidget() = 0;

    virtual ~IQTWidgetConfigurator() = default;
};

#endif // qtinterfaces.h
