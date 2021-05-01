/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QImage>
#include <QMainWindow>

#include <cryptopp/rsa.h>
#include <memory>
#include <string_view>

#include "ui_MainWindow.h"
#include "db/data/Author.hpp"

namespace window {
/**
 * @brief Main application window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    static constexpr std::string_view SelectImageFormatFilter { "*.jpg;" };
    static constexpr int SimilarityThreashold { 30 };

public:
    /**
     * @brief Construct default window.
     * @param parent Parent of the window, nullptr for no parent.
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Triggered when setting button is clicked.
     */
    void onSettingClicked();
    /**
     * @brief Triggered when user try to load image.
     */
    void onLoadImage();
    /**
     * @brief Triggered when user try to verify the image.
     */
    void onVerifyImage();

private:
    /**
     * @brief Load QSS from selected path.
     */
    void loadStylesheet();
    /**
     * @brief Extra steps to initialize the UI.
     */
    void initUI();
    /**
     * @brief Extract signature from image.
     * @return Extracted signature from image.
     */
    std::vector<std::byte> loadDataFromImage();
    /**
     * @brief Decode signature extracted.
     * @param signature Signature to decode.
     * @return Decoded signature in bytes.
     * @throw std::exception if any error occurred in the byte stream such as invalid or corrupted.
    */
    std::vector<std::byte> decodeSignature(const std::vector<std::byte> &signature);

private:
    /**
     * @brief UI generated via Qt Designer.
     */
    std::unique_ptr<Ui::MainWindow> ui_;
    /**
     * @brief Image to sign.
     */
    QImage targetImage_;
    /**
     * @brief Path to the image.
     */
    QString imagePath_;
    /**
     * @brief Selected author's public key.
     */
    CryptoPP::RSA::PublicKey pbKey_;
    /**
     * @brief Author information to sign.
     */
    db::data::Author author_;
};
}