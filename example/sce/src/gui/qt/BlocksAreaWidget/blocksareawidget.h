#ifndef BLOCKSAREAWIDGET_H
#define BLOCKSAREAWIDGET_H

#include <QTabWidget>
#include "runstopbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BlocksAreaWidget;
}
QT_END_NAMESPACE

///  \class BlocksAreaWidget
///  \brief A widget that organizes blocks in the form of tabs
class BlocksAreaWidget : public QTabWidget {
  Q_OBJECT

 public:
  explicit BlocksAreaWidget(QWidget* parent = nullptr);
  ~BlocksAreaWidget() override;

  [[nodiscard]] QPushButton* CornerButton() const;

 private:
  RunStopButton* button_;
  Ui::BlocksAreaWidget* ui;
};

#endif  // BLOCKSAREAWIDGET_H
