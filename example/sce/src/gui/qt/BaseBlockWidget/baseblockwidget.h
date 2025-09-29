#ifndef BASEBLOCKWIDGET_H
#define BASEBLOCKWIDGET_H

#include <QWidget>
#include "abstractsettingswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BaseBlockWidget;
}
QT_END_NAMESPACE

/**
 * \class BaseBlockWidget
 * \brief Class to store settings and stats/logs widgets
 * of emulation block
 */
class BaseBlockWidget : public QWidget {
  Q_OBJECT

 public:
  explicit BaseBlockWidget(QWidget* parent = nullptr);
  ~BaseBlockWidget() override;

  /**
   * \brief SetSettingsWidget
   *
   * Takes the given AbstractSettingsWidget and sets it to the settings frame of the block
   *
   * Does nothing if frame children count is not 1 or 2
   *
   * \attention Be careful: when a widget is already set, it will be replaced with the new one,
   * old values will be deleted.
   *
   * \param widget A pointer to an AbstractSettingsWidget that will be set for this block
   *
   * \see AbstractSettingsWidget
   */
  void SetSettingsWidget(AbstractSettingsWidget* widget);

  void SetStatsWidget(QWidget* widget);

 private:
  Ui::BaseBlockWidget* ui_;
};

#endif  // BASEBLOCKWIDGET_H
