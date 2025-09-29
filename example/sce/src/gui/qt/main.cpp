#include "mainwindow.h"

#include <QApplication>

#include "baseblockwidget.h"
#include "blocksareawidget.h"

#include "emulation_worker.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  auto* area = qobject_cast<BlocksAreaWidget*>(w.centralWidget());
  EmulationWorker worker;
  worker.SetRelatedBlocks(area);
  w.show();
  return QApplication::exec();
}