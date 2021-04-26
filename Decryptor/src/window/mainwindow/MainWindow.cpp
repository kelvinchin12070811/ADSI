/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>

#include <array>
#include <bitset>
#include <fstream>
#include <stdexcept>
#include <string>

#include <boost/range/irange.hpp>
#include <boost/process.hpp>
#include <fmt/format.h>

#include "MainWindow.hpp"
#include "codec/DefaultCodecFactory.hpp"
#include "utils/DCT.hpp"
#include "utils/StylesManager.hpp"
#include "window/setting/Setting.hpp"

namespace window {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_ { std::make_unique<Ui::MainWindow>() }
{
    ui_->setupUi(this);
    initUI();
    loadStylesheet();

#ifdef DEBUG
    std::fstream test;
    test.open("./perceptual_hash.exe", std::ios::in);
    if (test.is_open()) {
        qDebug() << "Found hash executable";
        test.close();
    }
    if (QFileInfo::exists("./jsteg.exe")) qDebug() << QStringLiteral("Found jsteg executable");
#endif // DEBUG
}

void MainWindow::onSettingClicked()
{
    auto dialog = std::make_unique<window::Setting>();
    dialog->exec();
}

void MainWindow::onLoadImage()
{
    imagePath_ = QFileDialog::getOpenFileName(this, "Select image to verify", QString {},
                                                  QStringLiteral("*.jpg"));
    if (imagePath_.isEmpty()) return;

    qDebug() << imagePath_;
    targetImage_.load(imagePath_);
    ui_->labImagePreview->setImage(&targetImage_);
}

void MainWindow::onVerifyImage()
{
    if (targetImage_.isNull()) {
        QMessageBox::information(this, "Select an image first.",
                                 "You need to load an image in first in order to verify anything.");
        return;
    }

    std::vector<std::byte> signature;

    try {
        signature = loadDataFromImage();
        signature = decodeSignature(signature);
    } catch (const std::exception &e) {
        qDebug() << e.what();
    }

#ifdef DEBUG
    std::ofstream outDebug;
    outDebug.open("_debug.dat", std::ios::binary | std::ios::out);
    if (outDebug.is_open()) {
        outDebug.write(reinterpret_cast<const char *>(signature.data()), signature.size());
        outDebug.close();
    }
#endif
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

std::vector<std::byte> MainWindow::loadDataFromImage()
{
    namespace bp = boost::process;
    bp::child signExtractor { fmt::format("jsteg.exe reveal \"{}\" \"{}.reveal.txt\"",
                                          imagePath_.toStdString(), imagePath_.toStdString()) };
    signExtractor.wait();

    std::ifstream reader;
    reader.open(fmt::format("{}.reveal.txt", imagePath_.toStdString()), std::ios::in);
    if (!reader.is_open()) throw std::runtime_error { "Invalid file" };

    std::string buffer;
    std::getline(reader, buffer);
    std::getline(reader, buffer);

    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };

    auto base64Dec = facCodec->createDefaultB2TDecoder(buffer);
    base64Dec->execute();
    return base64Dec->getCodecResult();
}

std::vector<std::byte> MainWindow::decodeSignature(const std::vector<std::byte> &signature)
{
    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto decompressor = facCodec->createDefaultDecompressCoder(signature);
    decompressor->execute();
    return decompressor->getCodecResult();
}
}

namespace boost {
#ifdef BOOST_NO_EXCEPTIONS
void throw_exception(std::exception const &e)
{
    throw 11; // or whatever
};
#endif
} // namespace boost
