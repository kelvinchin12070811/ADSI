/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QDialog>

#include <memory>

#include "ui_NewPasswordField.h"

namespace window {
/**
 * @brief Dialog that allow user to enter new password for encryption.
 */
class NewPasswordField : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct default dialog of the NewPasswordField.
     * @param parent Parent of the dialog, nullptr if no parent assigned.
     */
    explicit NewPasswordField(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Triggered when user enter text in re-enter password field.
     * @param value New value entered by user.
     */
    void onReenterPasswordChanged(QString value);

private:
    /**
     * @brief Ui elements of the dialog.
     */
    std::unique_ptr<Ui::NewPasswordField> ui_ { std::make_unique<Ui::NewPasswordField>() };
};
}