/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "window/passwordfield/EnterPasswordField.hpp"
#include "utils/StylesManager.hpp"

namespace window {
EnterPasswordField::EnterPasswordField(QWidget *parent) : QDialog(parent)
{
    ui_->setupUi(this);

    utils::StylesManager::getInstance().applyStylesheets(
            this, { ":/Themes/Default/AuthorInfoEditor.qss" });
}

std::optional<QString> EnterPasswordField::getPassword()
{
    return confirmed_ ? std::make_optional(ui_->lnedPassword->text()) : std::nullopt;
}

void EnterPasswordField::onConfirmResult()
{
    if (ui_->lnedPassword->text().isEmpty()) return;
    confirmed_ = true;
    this->close();
}
}