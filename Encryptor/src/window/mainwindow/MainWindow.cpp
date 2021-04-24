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
    auto result = dialog->getSelectedKey();
    if (result == std::nullopt) return;

    auto &[author, key] = *result;
    this->author_ = std::move(author);
    std::unique_ptr<key_generator::ICryptoKeyGeneratorFactory> facKey {
        std::make_unique<key_generator::DefaultCryptoKeyGeneratorFactory>()
    };

    pbKey_ = facKey->createDefaultPublicASymEncryptionKey(*key);
    prKey_ = facKey->createDefaultPrivateASymEncryptionKey(*key);
    pbKey_->generate();
    prKey_->generate();

    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto hashCodec = facCodec->createDefaultHashEncoder();

    auto rawPbKey = reinterpret_cast<key_generator::PublicRSACryptoKeyGenerator *>(pbKey_.get())->getPublicKey();
    auto rawPrKey = reinterpret_cast<key_generator::PrivateRSACryptoKeyGenerator *>(prKey_.get())->getPrivatekey();
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

    qDebug() << QString::fromStdString(
            fmt::format("Selected author's name: {}", this->author_.authorName));
}

void MainWindow::onBtnSignAndExport()
{
    auto result = QMessageBox::warning(this, "Operation will take very long time!",
                                       "This operation will take minutes to complete according to "
                                       "the size of the image, are you sure to continue?",
                                       QMessageBox::Button::Yes | QMessageBox::Button::No);

    if (result != QMessageBox::Button::Yes) return;

    auto prevWindowTitle = this->windowTitle();
    const auto titleTemplate = fmt::format("{} - {{}}", prevWindowTitle.toStdString());
    QApplication::setOverrideCursor(Qt::CursorShape::WaitCursor);

    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto signer =
            facCodec->createDefaultImageSigner(targetImage_, pbKey_.get(), prKey_.get(), &author_);
    auto connection = connect(
            signer.get(), &codec::ImageSignCodec::progressUpdated,
            [this, &titleTemplate](float progress) {
                auto percent = fmt::format("Signing... (Progress: {:.{}f}%)", progress, 2);
                this->setWindowTitle(QString::fromStdString(fmt::format(titleTemplate, percent)));
            });
    signer->execute();
    auto signingReceipt = signer->getSigningReceipt();
    auto signedImage = signer->getEncodedImage();

    qDebug() << QString::fromStdString(signingReceipt);
    disconnect(connection);
    this->setWindowTitle(prevWindowTitle);
    QApplication::restoreOverrideCursor();
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