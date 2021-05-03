/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QDebug>
#include <QLabel>
#include <QFileDialog>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include <boost/scope_exit.hpp>
#include <fmt/format.h>

#include "window/imgcomparetool/ImgCompareTool.hpp"
#include "utils/StylesManager.hpp"

namespace window {
ImgCompareTool::ImgCompareTool(QWidget *parent): QDialog(parent)
{
    ui_->setupUi(this);

    utils::StylesManager::getInstance().applyStylesheets(
            this,
            { ":/Themes/Default/AuthorInfoEditor.qss", ":/Themes/Default/ImgCompareTool.qss" });
}

void ImgCompareTool::onLoadLeftImg()
{
    auto path = QFileDialog::getOpenFileName(this, "Select Image", {},
                                             QStringLiteral("*.jpg;*.jpeg;*.png"));
    if (path.isEmpty()) return;

    lImgPath = std::move(path);
    QPixmap tmp;
    tmp.load(lImgPath);
    ui_->labLeftCompareImg->setPixmap(tmp);
}

void ImgCompareTool::onLoadRightImg()
{
    auto path = QFileDialog::getOpenFileName(this, "Select Image", {},
                                             QStringLiteral("*.jpg;*.jpeg;*.png"));
    if (path.isEmpty()) return;

    rImgPath = std::move(path);
    QPixmap tmp;
    tmp.load(rImgPath);
    ui_->labRightCompareImg->setPixmap(tmp);
}

void ImgCompareTool::onCompare()
{
    if (lImgPath.isEmpty() || rImgPath.isEmpty()) return;

    QApplication::setOverrideCursor(Qt::CursorShape::WaitCursor);

    BOOST_SCOPE_EXIT_ALL(&, this) { QApplication::restoreOverrideCursor(); };

    std::string argsHasher { fmt::format("./perceptual_hash.exe \"{}\" --hash",
                                         lImgPath.toStdString()) };
    boost::process::ipstream soutHasher;
#if defined(WIN32) && !defined(DEBUG)
    boost::process::child hasher { argsHasher, boost::process::std_out > soutHasher,
                                   boost::process::windows::create_no_window };
#else
    boost::process::child hasher { argsHasher, boost::process::std_out > soutHasher};
#endif // defined(WIN32) && !defined(DEBUG)
    std::string hash;
    std::getline(soutHasher, hash);
    boost::trim(hash);
    qDebug() << QString::fromStdString(hash);

    argsHasher =
            fmt::format("./perceptual_hash.exe \"{}\" --compare {}", rImgPath.toStdString(), hash);
    soutHasher = decltype(soutHasher) {};
#if defined(WIN32) && !defined(DEBUG)
    hasher = decltype(hasher) { argsHasher, boost::process::std_out > soutHasher,
                                boost::process::windows::create_no_window };
#else
    hasher = decltype(hasher) { argsHasher, boost::process::std_out > soutHasher };
#endif // defined(WIN32) && !defined(DEBUG)
    std::string rwMatchResult;
    std::getline(soutHasher, rwMatchResult);
    boost::trim(rwMatchResult);
    int matchResult = boost::lexical_cast<int>(rwMatchResult);
    qDebug() << matchResult;

    std::string message { fmt::format("Differences: {}, Matched: {}", matchResult,
                                      matchResult <= 30 ? "Yes" : "No") };
    ui_->labResult->setText(QString::fromStdString(message));
}
}