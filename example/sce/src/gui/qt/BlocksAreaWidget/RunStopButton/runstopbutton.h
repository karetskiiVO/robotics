#ifndef RUNSTOPBUTTON_H
#define RUNSTOPBUTTON_H

#include <QPushButton>

class RunStopButton : public QPushButton {
 public:
  RunStopButton(QWidget* parent = nullptr);

 private slots:
  void Toggle(bool checked);
};

#endif  // RUNSTOPBUTTON_H
