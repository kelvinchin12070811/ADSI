/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QGuiApplication>
#include <QScreen>
#include <QMessageBox>

#include "window/mainwindow/MainWindow.hpp"

namespace window
{
    MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent), ui{ std::make_unique<Ui::MainWindow>() }
    {
        ui->setupUi(this);

        auto largeWidth = QGuiApplication::primaryScreen()->size().width();
        ui->splitSidebar->setStretchFactor(ui->splitSidebar->indexOf(ui->sidebarPanel), 0);
        ui->splitSidebar->setStretchFactor(ui->splitSidebar->indexOf(ui->clientAreaPanel), 1);
        ui->sidebarPanel->resize(200, ui->sidebarPanel->height());

        connect(ui->btnImageLoad, &QPushButton::clicked, [this]() {
            QMessageBox::information(this, "Hello", "Hello World!");
        });
    }
}