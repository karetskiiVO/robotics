#include "blocksareawidget.h"
#include "ui_blocksareawidget.h"

BlocksAreaWidget::BlocksAreaWidget(QWidget* parent)
    : QTabWidget(parent), ui(new Ui::BlocksAreaWidget) {
  button_ = new RunStopButton(this);
  setCornerWidget(button_);
  ui->setupUi(this);
}

BlocksAreaWidget::~BlocksAreaWidget() {
  delete ui;
}

QPushButton* BlocksAreaWidget::CornerButton() const {
  return button_;
}
