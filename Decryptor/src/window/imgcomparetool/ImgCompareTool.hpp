/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QDialog>

#include <memory>

#include "ui_ImgCompareTool.h"

namespace window {
class ImgCompareTool : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Create dialogue.
     * @param parent 
     */
    explicit ImgCompareTool(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Triggered when tried to load left image.
     */
    void onLoadLeftImg();
    /**
     * @brief Triggered when tried to load right image.
     */
    void onLoadRightImg();
    /**
     * @brief Triggered when tried to compare image.
     */
    void onCompare();

private:
    /**
     * @brief User interface of the image comparison tool.
     */
    std::unique_ptr<Ui::ImgCompareTool> ui_ { std::make_unique<Ui::ImgCompareTool>() };
    /**
     * @brief Location of left image to compare.
     */
    QString lImgPath;
    /**
     * @brief Location of right image to compare.
     */
    QString rImgPath;
};
}