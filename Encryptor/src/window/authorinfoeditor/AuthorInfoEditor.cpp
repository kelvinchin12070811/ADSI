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
#include "window/authorinfoeditor/AuthorDetailsEditor.hpp"
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

    if (std::uint32_t curAuthorCount { static_cast<uint32_t>(ui_->authorList->items().count()) };
        curAuthorCount > lastAuthorCount_) {
        lastAuthorCount_ = curAuthorCount;
        return;
    }

    QMessageBox::information(this, this->windowTitle(),
                             QStringLiteral("Name \"%1\" already exist.").arg(stateAuthorName_));
}

void AuthorInfoEditor::onEditAuthorDetails(const QModelIndex &idxItem)
{
    auto dialog = std::make_unique<window::AuthorDetailsEditor>(
            idxItem.data(Qt::DisplayRole).toString(), this);
    dialog->exec();
    auto result = dialog->result();
    if (!result.has_value()) return;

    auto *db = &db::DBManager::getInstance().storage();
    db->update(*result);
    ui_->authorList->clear();
    loadData();
}

void AuthorInfoEditor::onAuthorNameChanged()
{
    using namespace sqlite_orm;

    auto curIdx = ui_->authorList->currentItem();
    auto curText = ui_->authorList->currentText();
    auto *db = &db::DBManager::getInstance().storage();

    auto author = db->get_all<db::data::Author>(order_by(&db::data::Author::authorID).desc(),
                                                limit(1, offset(curIdx)));
    if (author.empty()) return;
    db::data::Author newAuthor { *author.begin() };
    newAuthor.authorName = curText.toStdString();
    db->update(newAuthor);
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

    lastAuthorCount_ = ui_->authorList->items().count();
}

void AuthorInfoEditor::initConnections()
{
    connect(ui_->authorList, &KEditListWidget::added, this, &AuthorInfoEditor::onNewAuthorAdded);
    connect(ui_->authorList->addButton(), &QPushButton::clicked, this,
            &AuthorInfoEditor::onAddButtonClicked);
    connect(ui_->authorList->lineEdit(), &QLineEdit::textChanged,
            [this](const QString &value) { stateAuthorName_ = value; });
    connect(ui_->authorList->listView(), &QListView::doubleClicked, this,
            &AuthorInfoEditor::onEditAuthorDetails);
    connect(ui_->authorList, &KEditListWidget::changed, this, &AuthorInfoEditor::onAuthorNameChanged);
}
}
