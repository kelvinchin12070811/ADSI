/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "utils/StylesManager.hpp"
#include "window/passwordfield/NewPasswordField.hpp"

namespace window {
NewPasswordField::NewPasswordField(QWidget *parent) : QDialog(parent)
{
    ui_->setupUi(this);

    utils::StylesManager::getInstance().applyStylesheets(
            this, { ":/Themes/Default/NewPasswordField.qss" });
}
}