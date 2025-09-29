#include <QPushButton>

#include "baseblockwidget.h"
#include "ui_baseblockwidget.h"
#include "qtinterfaces.h"

BaseBlockWidget::BaseBlockWidget(QWidget* parent)
    : QWidget(parent), ui_(new Ui::BaseBlockWidget) {
  ui_->setupUi(this);
  ui_->splitter->setCollapsible(1, false);
  ui_->settingsButtonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
}

BaseBlockWidget::~BaseBlockWidget() {
  delete ui_;
}

void BaseBlockWidget::SetStatsWidget(QWidget* widget) {
  if (widget == nullptr) {
    return;
  }
  if (const auto child  = ui_->statsAreaLayout->takeAt(1); child) {
    delete child->widget();
    delete child;
  }
  ui_->statsAreaLayout->addWidget(widget);
}


void BaseBlockWidget::SetSettingsWidget(AbstractSettingsWidget* widget) {
  if (widget == nullptr) {
    return;
  }

  switch (ui_->settingsFrameLayout->count()) {
    case 1:
      ui_->settingsFrameLayout->insertWidget(0, widget, 0);
      break;
    case 2:
      if (auto child = ui_->settingsFrameLayout->takeAt(0); child != nullptr) {
        if (child->widget() != nullptr) {
          disconnect(child->widget(), nullptr, nullptr, nullptr);
          delete child->widget();
        }
        delete child;
      }
      ui_->settingsFrameLayout->insertWidget(0, widget, 0);
      break;
    default:
      return;
  }

  auto* apply = ui_->settingsButtonBox->button(QDialogButtonBox::Apply);
  const auto* reset = ui_->settingsButtonBox->button(QDialogButtonBox::Reset);
  connect(apply, &QPushButton::clicked, this, [widget, apply] {
    widget->saveSettings();
    apply->setDisabled(true);
  });
  connect(reset, &QPushButton::clicked, this, [widget, apply] {
    widget->restoreSettings();
    apply->setDisabled(true);
  });
  connect(widget, &AbstractSettingsWidget::settingsChanged, this,
          [apply] { apply->setEnabled(true); });
}
