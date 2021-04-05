/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>

#include <stdexcept>

#include "db/DBManager.hpp"
#include "utils/StylesManager.hpp"
#include "window/authorinfoeditor/AuthorInfoEditor.hpp"
#include "window/mainwindow/MainWindow.hpp"
#include "window/setting/Setting.hpp"

namespace window {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_ { std::make_unique<Ui::MainWindow>() }
{
    ui_->setupUi(this);
    initUI();
    loadStylesheet();
    auto *storage = &db::DBManager::getInstance();
    storage->initDB();

    try {
        using namespace sqlite_orm;
        auto author = storage->storage().get_all<db::data::Author>(
                where(c(&db::data::Author::authorName) == "Sakura Miyamoto"));
        if (author.empty()) throw std::out_of_range { "No entries found." };
        qDebug() << author.begin()->authorID;
        qDebug() << QString::fromStdString(author.begin()->authorName);
        qDebug() << QString::fromStdString(author.begin()->authorEmail);
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }
}

void MainWindow::onBtnLoadImgClicked()
{
    auto imgPath = QFileDialog::getOpenFileName(this, tr("Select Image"), {},
                                                { SelectImageFormatFilter.data() });
    if (imgPath.isEmpty()) return;
    targetImage_.load(imgPath);
    ui_->labImagePreview->setImage(&targetImage_);
}

void MainWindow::onBtnSettingClicked()
{
    std::unique_ptr<QDialog> dilSetting = std::make_unique<window::Setting>();
    dilSetting->exec();
}

void MainWindow::onBtnLoadKeyClicked()
{
    std::unique_ptr<QDialog> dialog = std::make_unique<window::AuthorInfoEditor>(this);
    dialog->exec();
}

void MainWindow::loadStylesheet()
{
    utils::StylesManager::getInstance().applyStylesheets(ui_->centralwidget);
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