/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QImage>
#include <QMainWindow>

#include <memory>
#include <string_view>

#include "ui_MainWindow.h"

namespace window {
/**
 * @brief Main application window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    static constexpr std::string_view SelectImageFormatFilter { "*.jpg;*.png" };

public:
    /**
     * @brief Construct default window.
     * @param parent Parent of the window, nullptr for no parent.
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onBtnLoadImgClicked();
    void onBtnSettingClicked();

private:
    /**
     * @brief Load QSS from selected path.
     */
    void loadStylesheet();
    /**
     * @brief Extra steps to initialize the UI.
     */
    void initUI();

private:
    /**
     * @brief UI generated via Qt Designer.
     */
    std::unique_ptr<Ui::MainWindow> ui_;
    /**
     * @brief Image to sign.
     */
    QImage targetImage;
};
}