/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QDialog>

#include <memory>

#include "ui_AuthorInfoEditor.h"

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
     * @brief Action triggered when new author entry is created.
     * @param inserted Author name that newly created.
     */
    void onNewAuthorAdded(QString inserted);
    /**
     * @brief Action triggered when new button is clicked.
     */
    void onAddButtonClicked();

private:
    /**
     * @brief Contains the UI elements of the AuthorInfoEditor.
     */
    std::unique_ptr<Ui::AuthorInfoEditor> ui_ { nullptr };
    /**
     * @brief State of user inputed author name.
     */
    QString stateAuthorName_;
};
}