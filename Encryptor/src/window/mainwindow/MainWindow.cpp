/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <stdexcept>
#include <QFile>
#include <QGuiApplication>
#include <QMessageBox>
#include <QScreen>

#include "window/mainwindow/MainWindow.hpp"
#include "window/setting/Setting.hpp"

namespace window {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui { std::make_unique<Ui::MainWindow>() }
{
    ui->setupUi(this);
    initUI();
    loadStylesheet();
}

void MainWindow::onBtnLoadImgClicked()
{
    QMessageBox::information(this, "Hello world!", "Hi there!");
}

void MainWindow::onBtnSettingClicked()
{
    auto dilSetting = std::make_unique<window::Setting>();
    dilSetting->exec();
}

void MainWindow::loadStylesheet()
{
    QFile styMaster { QStringLiteral(":/Themes/Default/Master.qss") };
    styMaster.open(QIODevice::OpenModeFlag::ReadOnly);

    if (!styMaster.isOpen())
        throw std::runtime_error { "Unable to load style file Master.qss" };

    auto stylesheet = QString { styMaster.readAll() };
    ui->centralwidget->setStyleSheet(stylesheet);
}

void MainWindow::initUI()
{
    constexpr int maxSize { 16777215 };

    auto largeWidth = QGuiApplication::primaryScreen()->size().width();
    ui->splitSidebar->setStretchFactor(ui->splitSidebar->indexOf(ui->sidebarPanel), 0);
    ui->splitSidebar->setStretchFactor(ui->splitSidebar->indexOf(ui->clientAreaPanel), 1);
    ui->sidebarPanel->resize(ui->sidebarPanel->maximumWidth(), ui->sidebarPanel->height());
    ui->sidebarPanel->setMaximumWidth(maxSize);
}
}