#ifndef DEFAULTSETTINGSWIDGET_H
#define DEFAULTSETTINGSWIDGET_H

#include "abstractsettingswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DefaultSettingsWidget;
}
QT_END_NAMESPACE

/**
 * \class DefaultSettingsWidget
 * \brief Simple settings widget with TextEdit for entering config in JSON format
 * \attention This widget does not check content for validity
 */
class DefaultSettingsWidget : public AbstractSettingsWidget {
    Q_OBJECT

   public:
    explicit DefaultSettingsWidget(QWidget* parent = nullptr);
    ~DefaultSettingsWidget();

  /**
   * \brief Retrieve the settings in JSON format
   * \return A string containing the settings in JSON format.
   */
 std::string SettingsJSON() const override;
 public slots:
  /**
   * \copydoc AbstractSettingsWidget::saveSettings()
   */
  void saveSettings() final;

    /**
     * \copydoc AbstractSettingsWidget::restoreSettings()
     */
    void restoreSettings() final;

protected:
    /**
     * \copydoc AbstractSettingsWidget::Init()
     */
    void Init() final;

private:
    QString old_;
    Ui::DefaultSettingsWidget* ui_;
};

#endif  // DEFAULTSETTINGSWIDGET_H