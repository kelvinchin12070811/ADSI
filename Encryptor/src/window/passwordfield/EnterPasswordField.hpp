/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QDialog>

#include <memory>
#include <optional>

#include "ui_EnterPasswordField.h"

namespace window {
/**
 * @brief Dialog that allow user to enter password.
 */
class EnterPasswordField : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Create new dialog for EnterPasswordField.
     * @param parent Parent of the dialog if available, nullptr if no parent.
     */
    explicit EnterPasswordField(QWidget *parent = nullptr);
    /**
     * @brief Get confirmed password form the dialog.
     * @return Password entered by the user of std::nullopt if not confirmed yet.
     */
    std::optional<QString> getPassword();

private slots:
    /**
     * @brief Triggered when user confirm the result of the dialog.
     */
    void onConfirmResult();

private:
    /**
     * @brief UI elements of the dialog.
     */
    std::unique_ptr<Ui::EnterPasswordField> ui_ { std::make_unique<Ui::EnterPasswordField>() };
    /**
     * @brief Determine if the result has been confirmed by the user.
     */
    bool confirmed_ { false };
};
}