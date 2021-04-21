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

#include <fmt/format.h>

#include "window/mainwindow/MainWindow.hpp"
#include "codec/DefaultCodecFactory.hpp"
#include "db/DBManager.hpp"
#include "generator/DefaultCryptoKeyGeneratorFactory.hpp"
#include "generator/PrivateRSACryptoKeyGenerator.hpp"
#include "generator/PublicRSACryptoKeyGenerator.hpp"
#include "utils/StylesManager.hpp"
#include "window/authorinfoeditor/AuthorInfoEditor.hpp"
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
    auto dilSetting = std::make_unique<window::Setting>();
    dilSetting->exec();
}

void MainWindow::onBtnLoadKeyClicked()
{
    auto dialog = std::make_unique<window::AuthorInfoEditor>(this);
    dialog->exec();
    auto key = dialog->getSelectedKey();
    if (key == nullptr) return;

    std::unique_ptr<key_generator::ICryptoKeyGeneratorFactory> facKey {
        std::make_unique<key_generator::DefaultCryptoKeyGeneratorFactory>()
    };
    auto pbKey = facKey->createDefaultPublicASymEncryptionKey(*key);
    auto prKey = facKey->createDefaultPrivateASymEncryptionKey(*key);

    pbKey->generate();
    prKey->generate();

    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto hashCodec = facCodec->createDefaultHashEncoder();

    auto rawPbKey = reinterpret_cast<key_generator::PublicRSACryptoKeyGenerator *>(pbKey.get())->getPublicKey();
    auto rawPrKey = reinterpret_cast<key_generator::PrivateRSACryptoKeyGenerator *>(prKey.get())->getPrivatekey();
    std::string strPbKey;
    std::string strPrKey;

    CryptoPP::StringSink pbKeySerializer { strPbKey };
    CryptoPP::StringSink prKeySerializer { strPrKey };

    rawPbKey.DEREncode(pbKeySerializer);
    rawPrKey.DEREncode(prKeySerializer);

    auto hashArray = [](const std::vector<std::byte> &data) {
        using namespace std::string_literals;
        return std::accumulate(
                data.begin(), data.end(), ""s,
                [](const auto &prev, const auto &cur) {
            return fmt::format("{}{:X}", prev, cur); });
    };

    hashCodec->setCodecData(strPbKey);
    hashCodec->execute();
    auto pbHash = hashArray(hashCodec->getCodecResult());

    hashCodec->setCodecData(strPrKey);
    hashCodec->execute();
    auto prHash = hashArray(hashCodec->getCodecResult());

    ui_->labPublicKeyDsp->setText(QString::fromStdString(pbHash));
    ui_->labPrivateKeyDsp->setText(QString::fromStdString(prHash));
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