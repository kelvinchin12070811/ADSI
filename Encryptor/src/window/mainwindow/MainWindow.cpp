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

#include <fstream>
#include <stdexcept>
#include <string>

#include <boost/process.hpp>

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

#ifdef DEBUG
    std::fstream test;
    test.open("./perceptual_hash.exe", std::ios::in);
    if (test.is_open()) {
        qDebug() << "Found hash executable";
        test.close();
    }
    if (QFileInfo::exists(QStringLiteral("./jsteg.exe"))) qDebug() << "Found jsteg executable";
#endif // DEBUG
}

void MainWindow::onBtnLoadImgClicked()
{
    auto imgPath = QFileDialog::getOpenFileName(this, tr("Select Image"), {},
                                                { SelectImageFormatFilter.data() });
    oriImagePath_ = imgPath;
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
    namespace bp = boost::process;

    if (targetImage_.isNull()) {
        QMessageBox::information(this, "No image selected", "Select an image first to sign");
        return;
    }

    if (prKey_ == nullptr) {
        QMessageBox::information(this, "No key selected", "Select a key first before sign");
        return;
    }

    auto outPath = QFileDialog::getSaveFileName(this, "Save file to...", QString {}, "*.jpg");
    if (outPath.isEmpty()) return;
    qDebug() << outPath;

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
    signer->execute();
    this->setWindowTitle(QString::fromStdString(fmt::format(titleTemplate, "Generating receipt")));
    auto signingReceipt = signer->getSigningReceipt();

    std::ofstream fileSigningReceipt;
    fileSigningReceipt.open(fmt::format("{}.sign", outPath.toStdString()));
    if (!fileSigningReceipt.is_open()) return;

    auto time = QDateTime::currentDateTimeUtc();
    std::string iso8601 { fmt::format(
            "{}-{}-{}T{}:{}:{}Z", time.date().year(), time.date().month(), time.date().day(),
            time.time().hour(), time.time().minute(), time.time().second()) };
    fileSigningReceipt << iso8601 << std::endl;
    fileSigningReceipt << signingReceipt << std::endl;

    bp::ipstream outHasher;
    bp::child hasher { fmt::format("./perceptual_hash.exe --hash \"{}\"", oriImagePath_.toStdString()),
                        bp::std_out > outHasher };
    hasher.wait();
    std::string pHash;
    std::getline(outHasher, pHash);
    qDebug() << QString::fromStdString(pHash);
    fileSigningReceipt << pHash;
    fileSigningReceipt.close();

    this->setWindowTitle(QString::fromStdString(fmt::format(titleTemplate, "Signing...")));
    bp::child imgSigner { fmt::format("jsteg.exe hide \"{}\" \"{}.sign\" \"{}\"",
                                      oriImagePath_.toStdString(), outPath.toStdString(),
                                      outPath.toStdString()) };
    imgSigner.wait();

    qDebug() << QString::fromStdString(signingReceipt);
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