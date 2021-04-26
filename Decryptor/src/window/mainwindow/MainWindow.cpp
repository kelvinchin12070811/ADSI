/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
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
                                                  QStringLiteral("*.png"));
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
    std::vector<std::byte> signature;
    signature.resize(sizeof(std::uint32_t));

    auto row = (targetImage_.height() / 8) + ((targetImage_.height() % 8) == 0 ? 0 : 1);
    auto col = (targetImage_.width() / 8) + ((targetImage_.width() % 8) == 0 ? 0 : 1);
    constexpr std::array<std::array<int, 2>, 4> posMidFreqCoefficients {
        { { 1, 4 }, { 2, 3 }, { 3, 2 }, { 4, 1 } }
    };
    std::bitset<8> buffer;
    int remainBits { 8 };
    std::uint32_t szData { 0 };

    for (auto grpY : boost::irange(row)) {
        for (auto grpX : boost::irange(col)) {
            std::vector<std::vector<float>> block;
            block.resize(8);
            for (auto &itr : block) {
                itr.resize(8);
                std::fill(itr.begin(), itr.end(), 0.f);
            }

            for (auto itrJ = block.begin(); itrJ != block.end(); itrJ++) {
                for (auto itrI = itrJ->begin(); itrI != itrJ->end(); itrI++) {
                    QPoint pos { static_cast<int>(std::distance(itrJ->begin(), itrI) * grpX),
                                 static_cast<int>(std::distance(block.begin(), itrJ) * grpY) };
                    auto pixColor = targetImage_.pixelColor(pos);

                    if (pixColor.isValid()) {
                        *itrI = pixColor.blueF();
                        continue;
                    }
                }
            }

            utils::DCT dct;
            auto dcted = dct.transfrom(block);

            for (auto idxCoefficient : boost::irange(posMidFreqCoefficients.size())) {
                if (remainBits == 0) {
                    signature.push_back(static_cast<std::byte>(buffer.to_ulong()));
                    buffer = { 0 };
                    remainBits = 8;

                    if (signature.size() == szData + 4ull) return signature;
                }

                if (signature.size() == 4) {
                    auto szData = *reinterpret_cast<std::uint32_t *>(signature.data());
                    if (szData <= 0) throw std::runtime_error { "Invalid length" };
                    if (szData > (row * col)) throw std::runtime_error { "Length overflow" };
                }

                const auto &[posX, posY] = posMidFreqCoefficients[idxCoefficient];
                std::bitset<sizeof(float)> coeficientBits { static_cast<std::uint64_t>(
                        dcted[posY][posX]) };
                buffer[8 - static_cast<size_t>(remainBits)] = coeficientBits[0];
                remainBits--;
            }
        }
    }
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