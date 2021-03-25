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
    : QMainWindow(parent), ui_ { std::make_unique<Ui::MainWindow>() }
{
    ui_->setupUi(this);
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
    ui_->centralwidget->setStyleSheet(stylesheet);
}

void MainWindow::initUI()
{
    constexpr int maxSize { 16777215 };

    auto largeWidth = QGuiApplication::primaryScreen()->size().width();
    ui_->splitSidebar->setStretchFactor(ui_->splitSidebar->indexOf(ui_->sidebarPanel), 0);
    ui_->splitSidebar->setStretchFactor(ui_->splitSidebar->indexOf(ui_->clientAreaPanel), 1);
    ui_->sidebarPanel->resize(ui_->sidebarPanel->maximumWidth(), ui_->sidebarPanel->height());
    ui_->sidebarPanel->setMaximumWidth(maxSize);
}
}