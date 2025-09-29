#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->menuButtonGroup, &QButtonGroup::idToggled, this, &MainWindow::toggleMenu);
    ui->menuButtonGroup->setExclusive(true);
    ui->clientsButton->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleMenu(int id, bool toggled) {
    static const auto buttonGroupSize = static_cast<int>(ui->menuButtonGroup->buttons().size());
    if (toggled) {
        ui->stackedWidget->setCurrentIndex(-id-buttonGroupSize);
    }
}
