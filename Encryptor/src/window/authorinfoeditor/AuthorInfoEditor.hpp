/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QDialog>

#include <memory>

#include "ui_AuthorInfoEditor.h"
#include "db/data/Author.hpp"

namespace window {
/**
 * @brief Dialog to select different authors and rsa key.
 */
class AuthorInfoEditor : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Create AuthorInfoEditor dialog.
     * @param parent parent of the dialog, nullptr for no parent.
     */
    explicit AuthorInfoEditor(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Triggered when cancel button is clicked.
     */
    void onBtnCancelClicked();
    /**
     * @brief Action triggered when new author entry is created.
     * @param inserted Author name that newly created.
     */
    void onNewAuthorAdded(QString inserted);
    /**
     * @brief Action triggered when new button is clicked.
     */
    void onAddButtonClicked();
    /**
     * @brief Triggered when an author info record is double clicked.
     * @param idxItem Index of item to edit.
     */
    void onEditAuthorDetails(const QModelIndex &idxItem);
    /**
     * @brief Triggered when a author name is changed.
     */
    void onAuthorNameChanged();
    /**
     * @brief Triggered when a author is removed.
     * @param text Author name that is removed.
     */
    void onRemoveAuthor(const QString &text);
    /**
     * @brief Triggered when user switched to other tab.
     * @param index Index of the tab user switched to.
     */
    void onChangedTab(int index);
    /**
     * @brief Triggered when user tend to generate new key.
     */
    void onNewKeyClicked();
    /**
     * @brief Triggered when user tend to remove selected key.
     */
    void onRemoveKey();

private:
    /**
     * @brief Additional steps to setup UI of AuthorInfoEditor.
     */
    void setupUI();
    /**
     * @brief Load required data from database and initialize the views.
     */
    void loadData();
    /**
     * @brief Initialize extra signals connection required to setup the UI.
     */
    void initConnections();
    /**
     * @brief Initialize RSA key list tab when user switched to it.
     */
    void switchedToKeyListTab();
    /**
     * @brief Load key data into the ui.
     * @param author Author's key to load.
     */
    void loadKeyData(const db::data::Author &author);
    /**
     * @brief Hash RSA key.
     * @param key Key to hash.
     * @return Digest of @p key.
     */
    std::string hashRSAKey(std::string_view key);

private:
    /**
     * @brief Contains the UI elements of the AuthorInfoEditor.
     */
    std::unique_ptr<Ui::AuthorInfoEditor> ui_ { nullptr };
    /**
     * @brief State of user inputed author name.
     */
    QString stateAuthorName_;
    /**
     * @brief Counter that keep tracking last amunt of author entries.
     */
    std::uint32_t lastAuthorCount_;
};
}