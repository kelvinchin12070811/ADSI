/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QDebug>
#include <QLineEdit>
#include <QListView>
#include <QMessageBox>

#include <algorithm>

#include "db/DBManager.hpp"
#include "utils/StylesManager.hpp"
#include "window/authorinfoeditor/AuthorInfoEditor.hpp"

namespace window {
AuthorInfoEditor::AuthorInfoEditor(QWidget *parent)
    : QDialog(parent), ui_ { std::make_unique<Ui::AuthorInfoEditor>() }
{
    ui_->setupUi(this);

    utils::StylesManager::getInstance().applyStylesheets(
            this, { ":/Themes/Default/AuthorInfoEditor.qss" });
    
    setupUI();
    loadData();
    initConnections();
}

void AuthorInfoEditor::onBtnCancelClicked()
{
    this->close();
}

void AuthorInfoEditor::setupUI()
{
    auto buttons = ui_->authorList->buttons();
    buttons = KEditListWidget::Button::Add | KEditListWidget::Button::Remove;
    ui_->authorList->setButtons(buttons);
    ui_->authorList->lineEdit()->setPlaceholderText("Enter author name");
    ui_->authorList->listView()->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
}

void AuthorInfoEditor::loadData()
{
    using namespace sqlite_orm;

    auto *db = &db::DBManager::getInstance().storage();
    for (const auto &itr :
         db->iterate<db::data::Author>(order_by(&db::data::Author::authorID).desc())) {
        ui_->authorList->insertItem(QString::fromStdString(itr.authorName));
    }
}
void AuthorInfoEditor::initConnections()
{
    connect(ui_->authorList, &KEditListWidget::added, this, &AuthorInfoEditor::onNewAuthorAdded);
    connect(ui_->authorList->addButton(), &QPushButton::clicked, this,
            &AuthorInfoEditor::onAddButtonClicked);
    connect(ui_->authorList->lineEdit(), &QLineEdit::textChanged,
            [this](const QString &value) { stateAuthorName_ = value; });
}

void AuthorInfoEditor::onNewAuthorAdded(QString inserted)
{
    using namespace sqlite_orm;
    auto *db = &db::DBManager::getInstance().storage();
    db::data::Author newAuthor;
    newAuthor.authorName = inserted.toStdString();

    try {
        db->insert(newAuthor);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, this->windowTitle(), e.what());
    }
}

void AuthorInfoEditor::onAddButtonClicked()
{
    using namespace sqlite_orm;

    try {
        auto authorName = stateAuthorName_.toStdString();
        auto *db = &db::DBManager::getInstance().storage();
        auto authors = db->get_all<db::data::Author>(
                where(c(&db::data::Author::authorName) == authorName));

        auto authorList = ui_->authorList->items();
        authorList.sort();
        auto result = std::binary_search(authorList.begin(), authorList.end(), stateAuthorName_,
                                   [](const auto &lhs, const auto &rhs) { return lhs < rhs; });

        if (result && authors.empty()) return;
        QMessageBox::critical(this, this->windowTitle(),
                              QStringLiteral("Name \"%1\" already exist.")
                                      .arg(QString::fromStdString(authorName)));
    } catch (const std::exception &e) {
        QMessageBox::critical(this, this->windowTitle(), e.what());
    }
}
}
