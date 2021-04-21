/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QMessageBox>
#include <QStyle>

#include "window/passwordfield/NewPasswordField.hpp"
#include "utils/StylesManager.hpp"

namespace window {
NewPasswordField::NewPasswordField(QWidget *parent) : QDialog(parent)
{
    ui_->setupUi(this);

    utils::StylesManager::getInstance().applyStylesheets(
            this, { ":/Themes/Default/NewPasswordField.qss" });
}

std::optional<std::reference_wrapper<const QString>> NewPasswordField::getPassword() const
{
    return confirmed_ ? std::make_optional(std::cref(password_)) : std::nullopt;
}

void NewPasswordField::onReenterPasswordChanged(QString value)
{
    if (ui_->lnedPassword->text().isEmpty() || ui_->lnedConfirmPassword->text().isEmpty()) {
        ui_->labErrorMessage->setProperty("isError", true);
        ui_->labErrorMessage->setText("Password field must not be empty");
        return;
    }

    if (ui_->lnedPassword->text() == ui_->lnedConfirmPassword->text()) {
        
        ui_->labErrorMessage->setText("Ok");
        ui_->labErrorMessage->setProperty("isError", false);
        ui_->labErrorMessage->style()->polish(ui_->labErrorMessage);
        return;
    }

    ui_->labErrorMessage->setText("Password did not match");
    ui_->labErrorMessage->setProperty("isError", true);
    ui_->labErrorMessage->style()->polish(ui_->labErrorMessage);
}

void NewPasswordField::onConfirmPassword()
{
    if (ui_->labErrorMessage->property("isError").toBool() == true) {
        QMessageBox::critical(this, "Passwords did not match",
                              "Passwords entered in both of the password field does not match");
        return;
    }

    password_ = ui_->lnedPassword->text();
    confirmed_ = true;
    this->close();
}
}