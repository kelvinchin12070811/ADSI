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

#include <fmt/format.h>
#include <bcrypt/BCrypt.hpp>

#include "window/authorinfoeditor/AuthorInfoEditor.hpp"
#include "codec/DefaultCodecFactory.hpp"
#include "db/DBManager.hpp"
#include "generator/DefaultCryptoKeyGeneratorFactory.hpp"
#include "utils/StylesManager.hpp"
#include "window/authorinfoeditor/AuthorDetailsEditor.hpp"
#include "window/passwordfield/NewPasswordField.hpp"

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
    if (curIdx < 0) return;

    auto curText = ui_->authorList->currentText();
    auto *dbManager = &db::DBManager::getInstance();
    auto author = dbManager->getAuthorByDistance(curIdx);

    if (!author.has_value()) return;
    db::data::Author newAuthor { *author };
    newAuthor.authorName = curText.toStdString();
    dbManager->updateAuthor(newAuthor);
}

void AuthorInfoEditor::onRemoveAuthor(const QString &text)
{
    auto *dbManager = &db::DBManager::getInstance();
    auto author = dbManager->getAuthorByName(text.toStdString());
    if (!author.has_value()) return;
    dbManager->removeAuthorById(author->authorID);
}

void AuthorInfoEditor::onChangedTab(int index)
{
    if (index == ui_->gtabAuthorInfo->indexOf(ui_->tabRSAKeys)) switchedToKeyListTab();
}

void AuthorInfoEditor::onNewKeyClicked()
{
    if (ui_->authorList->currentItem() < 0) {
        QMessageBox::critical(this, this->windowTitle(), "No author selected!");
        return;
    }

    auto pwDialog = std::make_unique<window::NewPasswordField>(this);
    pwDialog->exec();
    auto password = pwDialog->getPassword();
    if (password == std::nullopt) return;

    ui_->btnNewRSA->setDisabled(true);
    ui_->btnRemoveRSA->setDisabled(true);

    std::unique_ptr<key_generator::ICryptoKeyGeneratorFactory> facKey {
        std::make_unique<key_generator::DefaultCryptoKeyGeneratorFactory>()
    };
    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto dbManager = &db::DBManager::getInstance();
    auto symKey = facKey->createDefaultSymEncryptionKey(password->get().toStdString());
    auto asymKey = facKey->generateASymParams();
    auto curAuthor = dbManager->getAuthorByName(ui_->authorList->currentText().toStdString());
    
    db::data::KeyStore keyStore;
    keyStore.keyID = std::numeric_limits<uint32_t>::max();
    keyStore.authorID = curAuthor->authorID;
    keyStore.keyPasswordHash = BCrypt::generateHash(password->get().toStdString());
    
    auto symCodec = facCodec->createDefaultSymCryptoEncoder(facKey->serializeKeyParams(*asymKey),
                                                            symKey.get());
    symCodec->execute();

    auto binary2TextEncoder = facCodec->createDefaultB2TEncoder(symCodec->getCodecResult());    
    binary2TextEncoder->execute();

    const auto &encAsymKey = binary2TextEncoder->getCodecResult();
    keyStore.keyParams.reserve(encAsymKey.size());
    std::transform(encAsymKey.begin(), encAsymKey.end(), std::back_inserter(keyStore.keyParams),
                   [](const auto &itr) { return static_cast<char>(itr); });
    dbManager->insertNewKeyForAuthor(keyStore);

    ui_->btnNewRSA->setDisabled(false);
    ui_->btnRemoveRSA->setDisabled(false);

    ui_->lsvwKeys->clear();
    loadKeyData(*curAuthor);
}

void AuthorInfoEditor::onRemoveKey()
{
    auto idx = ui_->lsvwKeys->currentIndex();
    if (idx.row() < 0) return;

    auto usrConfirm = QMessageBox::warning(
            this, "Are you sure to continue?", "The deleted key is not recoverable and hence lost.",
            QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

    if (usrConfirm == QMessageBox::StandardButton::No) return;

    auto dbManager = &db::DBManager::getInstance();
    auto author = dbManager->getAuthorByName(ui_->authorList->currentText().toStdString());
    if (author == std::nullopt) return;

    auto key = dbManager->getAuthorKeyByDistance(author->authorID, idx.row());
    if (key == std::nullopt) return;

    dbManager->removeKeyByID(key->keyID);

    ui_->lsvwKeys->clear();
    loadKeyData(*author);
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
    connect(ui_->authorList, &KEditListWidget::removed, this, &AuthorInfoEditor::onRemoveAuthor);
}

void AuthorInfoEditor::switchedToKeyListTab()
{
    auto curIdx = ui_->authorList->currentItem();
    auto curText = ui_->authorList->currentText();

    ui_->lsvwKeys->clear();

    if (curIdx < 0) {
        ui_->btnNewRSA->setDisabled(true);
        ui_->btnRemoveRSA->setDisabled(true);
    } else {
        ui_->btnNewRSA->setDisabled(false);
        ui_->btnRemoveRSA->setDisabled(false);
    }

    auto *dbManager = &db::DBManager::getInstance();
    auto author = dbManager->getAuthorByName(curText.toStdString());
    if (!author.has_value()) return;

    loadKeyData(*author);
}

void AuthorInfoEditor::loadKeyData(const db::data::Author &author)
{
    auto dbManager = &db::DBManager::getInstance();
    
    for (const auto &key : dbManager->iterateKeysByAuthor(author.authorID))
        ui_->lsvwKeys->addItem(QString::fromStdString(hashRSAKey(key.keyParams)));
}

std::string AuthorInfoEditor::hashRSAKey(std::string_view key)
{
    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto hashCodec = facCodec->createDefaultHashEncoder({});
    hashCodec->setCodecData(key);
    hashCodec->execute();
    const auto &hash = hashCodec->getCodecResult();
    std::string strHash { std::accumulate(
            hash.begin(), hash.end(), std::string {}, [](auto prev, auto cur) {
                return fmt::format("{}{:X}", prev, static_cast<int>(cur));
            }) };
    return strHash;
}
}
