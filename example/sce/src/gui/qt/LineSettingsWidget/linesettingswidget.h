#ifndef LINESETTINGSWIDGET_H
#define LINESETTINGSWIDGET_H

#include <gui/qt/SettingsWidget/settingswidget.h>

QT_BEGIN_MOC_NAMESPACE
namespace Ui {
class LineSettingsWidget;
}
QT_END_NAMESPACE


/**
 * @class LineSettingsWidget
 * @brief A widget to store settings of Uplink and Downlink blocks
 */
class LineSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit LineSettingsWidget(QWidget *parent = nullptr);
    ~LineSettingsWidget();

private:
    Ui::LineSettingsWidget *ui;
};

#endif // LINESETTINGSWIDGET_H
