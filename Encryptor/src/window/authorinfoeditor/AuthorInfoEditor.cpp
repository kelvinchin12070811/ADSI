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
    auto *dbManager = &db::DBManager::getInstance();
    db::data::Author newAuthor;
    newAuthor.authorName = inserted.toStdString();
    dbManager->insertNewAuthor(std::move(newAuthor));
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

    auto *dbManager = &db::DBManager::getInstance();
    dbManager->updateAuthor(*result);
    ui_->authorList->lineEdit()->setText(QString::fromStdString(result->authorName));
}

void AuthorInfoEditor::onAuthorNameChanged()
{
    using namespace sqlite_orm;

    auto curIdx = ui_->authorList->currentItem();
    auto curText = ui_->authorList->currentText();
    auto *dbManager = &db::DBManager::getInstance();

    auto author = dbManager->getAuthorByDistance(curIdx);
    if (!author.has_value()) return;
    db::data::Author newAuthor { *author };
    newAuthor.authorName = curText.toStdString();
    dbManager->updateAuthor(newAuthor);
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

    auto *dbManager = &db::DBManager::getInstance();
    for (const auto &itr : dbManager->iterateAuthor())
        ui_->authorList->insertItem(QString::fromStdString(itr.authorName));
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
