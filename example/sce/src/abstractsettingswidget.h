#ifndef ABSTRACTSETTINGSWIDGET_h
#define ABSTRACTSETTINGSWIDGET_h

#include <QWidget>

/**
 * \class AbstractSettingsWidget
 * \brief Interface for settings widget if QT is used
 */
class AbstractSettingsWidget : public QWidget {
  Q_OBJECT
 public:
  virtual ~AbstractSettingsWidget() = default;

  virtual std::string SettingsJSON() const = 0;

protected:
  explicit AbstractSettingsWidget(QWidget* parent = nullptr)
      : QWidget(parent) {}
  /**
  * \brief Initialize the widget and settings fields
  *
  * This method can be overwritten in inherited classes to initialize them,
  * especially when ui files are used to describe interface
  *
  * Should be called in inherited classes constructors
  *
  */
  virtual void Init() {}

  signals:
   /**
      * \brief Signal emitted when settings have changed.
      *
      * This signal should be emitted whenever the settings in the widget
      * are modified, allowing other components to respond to the changes.
      */
   Q_SIGNAL void settingsChanged();

  public slots:
   /**
     * \brief Save the current settings
     *
     * This slot should be implemented in derived classes to save current settings
     * for example to a persistent storage or private field
     */
   virtual void saveSettings() {}
  /**
   * \brief Restore settings to their last saved state
   *
   * This slot should be implemented in derived classes to restore the
   * settings of the widget to last saved state
   *
   * Should not emit settingsChanged
   * \see settingsChanged
   */
  virtual void restoreSettings() {}
};

#endif