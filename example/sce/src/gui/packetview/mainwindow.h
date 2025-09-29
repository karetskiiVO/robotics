#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QToolTip>
#include <algorithm>
#include <interfaces.h>
#include <vector>
#include <memory>

#include "QCustomPlot/qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SetupVisuals();
    void AdjustVisuals();
    void DrawPackets();

    void SetupConnections();

    void ReadPackets();
    void ParsePackets(const std::string& input);

    void CheckHover(const QPointF &pos);

private:
    Ui::MainWindow *ui;

    std::vector<Packet> packets_;
    std::vector<QCPItemRect*> rectangles_;
};
#endif // MAINWINDOW_H
