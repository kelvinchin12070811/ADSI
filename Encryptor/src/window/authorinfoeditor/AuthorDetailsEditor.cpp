/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "db/DBManager.hpp"
#include "utils/StylesManager.hpp"
#include "window/authorinfoeditor/AuthorDetailsEditor.hpp"

namespace window {
AuthorDetailsEditor::AuthorDetailsEditor(QString authorName, QWidget *parent)
    : QDialog(parent), authorName_ { authorName }, ui_ { std::make_unique<Ui::AuthorDetailsEditor>() }
{
    ui_->setupUi(this);
    loadData();
    utils::StylesManager::getInstance().applyStylesheets(
            this, { ":/Themes/Default/AuthorInfoEditor.qss" });
}

std::optional<db::data::Author> AuthorDetailsEditor::result()
{
    if (submitted_) return author_;
    return std::nullopt;
}

void AuthorDetailsEditor::loadData()
{
    auto *dbManager = &db::DBManager::getInstance();
    auto author = dbManager->getAuthorByName(authorName_.toStdString());
    
    if (!author.has_value()) {
        throw std::out_of_range {
            QStringLiteral("No author named \"%1\" found.").arg(authorName_).toUtf8().data()
        };
    }

    author_ = *author;
    ui_->lnedtName->setText(QString::fromStdString(author_.authorName));
    ui_->lnedtEmail->setText(QString::fromStdString(author_.authorEmail));
    ui_->lnedtPortfolio->setText(QString::fromStdString(author_.authorPortFolioURL));
}

void AuthorDetailsEditor::onBtnOkClicked()
{
    author_.authorName = ui_->lnedtName->text().toStdString();
    author_.authorEmail = ui_->lnedtEmail->text().toStdString();
    author_.authorPortFolioURL = ui_->lnedtPortfolio->text().toStdString();
    submitted_ = true;
    this->close();
}
}