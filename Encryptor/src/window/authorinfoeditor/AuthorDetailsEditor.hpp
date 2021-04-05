/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QDialog>

#include <memory>

#include "ui_AuthorDetailsEditor.h"

namespace window {
/**
 * @brief AuthorDetailsEditor is the dialog that allow user to edit the details of the selected author. These details
 * include author's email address and portfolio webside.
 */
class AuthorDetailsEditor : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Create dialog and set target author to edit his details.
     * @param authorName Target author to alther.
     * @param parent Indicate the parent of the dialog, nullptr if no parent.
     */
    explicit AuthorDetailsEditor(QString authorName, QWidget *parent = nullptr);

private:
    /**
     * @brief Additional steps to setup ui of the dialog.
     */
    void setupUI();

private:
    /**
     * @brief Represented as generated ui from Qt Designer.
     */
    std::unique_ptr<Ui::AuthorDetailsEditor> ui_;
    /**
     * @brief Name of author to edit his details.
     */
    QString authorName_;

};
}