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
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/irange.hpp>
#include <boost/process.hpp>
#include <boost/process/windows.hpp>
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
    namespace bp = boost::process;

    auto guardCursorChanger = []() {
        QApplication::setOverrideCursor(Qt::CursorShape::WaitCursor);
        return std::unique_ptr<int, std::function<void(int *)>> {
            new int {},
            [](int *ptr) {
                QApplication::restoreOverrideCursor();
                delete ptr;
            }
        };
    }();

    if (targetImage_.isNull()) {
        QMessageBox::information(this, "Select an image first.",
                                 "You need to load an image in first in order to verify anything.");
        return;
    }

    std::vector<std::byte> signature;

    try {
        signature = loadDataFromImage();

        if (signature.empty()) throw std::runtime_error { "Invalid file" };

        signature = decodeSignature(signature);
    } catch (const std::exception &e) {
        qDebug() << e.what();
        QMessageBox::information(this, "No valid signature found",
                                 "Image seems not been signed with ADSI or has been corrupted");
        return;
    }

    std::uint16_t szData { 0 };
    int szTotal { 0 };
    std::string pbKeyData;
    auto begSignature = reinterpret_cast<const char *>(signature.data());
    std::istringstream signatureReader{ { begSignature, begSignature + signature.size() } };

    signatureReader.read(reinterpret_cast<char *>(&szData), 2);
    szTotal += szData + 2;
    pbKeyData.resize(szData);
    signatureReader.read(pbKeyData.data(), szData);
    CryptoPP::StringSource source { pbKeyData, true };
    pbKey_.BERDecode(source);
    
    signatureReader.read(reinterpret_cast<char *>(&szData), 2);
    szTotal += szData + 2;
    author_.authorName.resize(szData);
    signatureReader.read(author_.authorName.data(), szData);

    signatureReader.read(reinterpret_cast<char *>(&szData), 2);
    szTotal += szData + 2;
    author_.authorEmail.resize(szData);
    signatureReader.read(author_.authorEmail.data(), szData);
    
    signatureReader.read(reinterpret_cast<char *>(&szData), 2);
    szTotal += szData + 2;
    author_.authorPortFolioURL.resize(szData);
    signatureReader.read(author_.authorPortFolioURL.data(), szData);
    
    std::vector<CryptoPP::byte> sign;
    signatureReader.read(reinterpret_cast<char *>(&szData), 2);
    sign.resize(szData);
    signatureReader.read(reinterpret_cast<char *>(sign.data()), sign.size());

    CryptoPP::RSASSA_PKCS1v15_SHA_Verifier verifier { pbKey_ };
    auto match = verifier.VerifyMessage(reinterpret_cast<CryptoPP::byte *>(signature.data()),
                                        szTotal, sign.data(), sign.size());

    std::string message { fmt::format(
            "Name: {}<br>Email: <a href='mailto:{}'>{}</a><br>Portfolio: <a "
            "href={}>{}</a><br>Verified: {}",
            author_.authorName, author_.authorEmail, author_.authorEmail,
            author_.authorPortFolioURL, author_.authorPortFolioURL, match ? "Yes" : "No") };

    std::unique_ptr<int, std::function<void(int *)>> guardApplyLabel {
        new int {},
        [&message, this](int *ptr) {
            ui_->labAuthorInfo->setText(QString::fromStdString(message));
            delete ptr;
        }
    };

    if (!QFile::exists(QString::fromStdString(fmt::format("{}.sign", imagePath_.toStdString()))))
        return;

    std::ifstream readerSignReceipt;
    readerSignReceipt.open(fmt::format("{}.sign", imagePath_.toStdString()), std::ios::in);
    std::string hash;
    std::getline(readerSignReceipt, hash);
    std::getline(readerSignReceipt, hash);
    std::getline(readerSignReceipt, hash);

    boost::trim(hash);
    qDebug() << QString::fromStdString(hash);

    auto argsPHasher = fmt::format("./perceptual_hash.exe \"{}\" --compare \"{}\"",
                                   imagePath_.toStdString(), hash);
    bp::ipstream soutPHasher;
#if defined(WIN32) && !defined(DEBUG)
    bp::child pHasher { argsPHasher, bp::std_out > soutPHasher, bp::windows::create_no_window };
#else
    bp::child pHasher { argsPHasher, bp::std_out > soutPHasher };
#endif // defined(WIN32) && !defined(DEBUG)
    pHasher.wait();
    
    std::string rsltPHasher;
    std::getline(soutPHasher, rsltPHasher);
    boost::trim(rsltPHasher);
    qDebug() << QString::fromStdString(rsltPHasher);

    message += fmt::format("<br>Modified: {}",
                           boost::lexical_cast<int>(rsltPHasher) < SimilarityThreashold ? "No"
                                                                                        : "Yes");

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

    auto argsSignExtractor = fmt::format("jsteg.exe reveal \"{}\" \"{}.reveal.txt\"",
                                         imagePath_.toStdString(), imagePath_.toStdString());
#if defined(WIN32) && !defined(DEBUG)
    bp::child signExtractor { argsSignExtractor,
                              boost::process::windows::create_no_window };
#else
    bp::child signExtractor { argsSignExtractor };
#endif // defined(WIN32) && !defined(DEBUG)

    signExtractor.wait();

    std::ifstream reader;
    reader.open(fmt::format("{}.reveal.txt", imagePath_.toStdString()), std::ios::in);
    if (!reader.is_open()) throw std::runtime_error { "Invalid file" };

    std::string buffer;
    std::getline(reader, buffer);
    std::getline(reader, buffer);

    reader.close();
    QFile tempFile { QString::fromStdString(
            fmt::format("{}.reveal.txt", imagePath_.toStdString())) };
    tempFile.remove();

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
