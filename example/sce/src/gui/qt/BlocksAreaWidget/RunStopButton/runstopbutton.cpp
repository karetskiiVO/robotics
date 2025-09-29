#include "runstopbutton.h"

RunStopButton::RunStopButton(QWidget* parent) : QPushButton(parent) {
  Q_INIT_RESOURCE(resources);
  setIcon(QIcon(":/icons/run-icon"));
  setCheckable(true);
  setFlat(true);
  connect(this, &QPushButton::toggled, this, &RunStopButton::Toggle);
}

void RunStopButton::Toggle(bool checked) {
  if (checked) {
    setIcon(QIcon(":/icons/stop-icon"));
  } else {
    setIcon(QIcon(":/icons/run-icon"));
  }
}
