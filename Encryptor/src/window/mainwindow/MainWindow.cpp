#include "MainWindow.hpp"

#include "ui_MainWindow.h"

namespace window
{
    MainWindow::MainWindow(QMainWindow* parent) :
        QMainWindow(parent), ui(std::make_unique<Ui::MainWindow>())
    {
        ui->setupUi(this);
    }
}