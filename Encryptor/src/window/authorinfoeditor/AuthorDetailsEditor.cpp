/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "utils/StylesManager.hpp"
#include "window/authorinfoeditor/AuthorDetailsEditor.hpp"

namespace window {
AuthorDetailsEditor::AuthorDetailsEditor(QString authorName, QWidget *parent)
    : QDialog(parent), authorName_ { authorName }, ui_ { std::make_unique<Ui::AuthorDetailsEditor>() }
{
    ui_->setupUi(this);
    utils::StylesManager::getInstance().applyStylesheets(
            this, { ":/Themes/Default/AuthorInfoEditor.qss" });
}
}