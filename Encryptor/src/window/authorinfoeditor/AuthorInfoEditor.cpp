/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "utils/StylesManager.hpp"
#include "window/authorinfoeditor/AuthorInfoEditor.hpp"

namespace window {
AuthorInfoEditor::AuthorInfoEditor(QWidget *parent)
    : QDialog(parent), ui_ { std::make_unique<Ui::AuthorInfoEditor>() }
{
    ui_->setupUi(this);

    utils::StylesManager::getInstance().applyStylesheets(
            this, { ":/Themes/Default/AuthorInfoEditor.qss" });
    ui_->authorList->upButton()->hide();
    ui_->authorList->downButton()->hide();
}

void AuthorInfoEditor::onBtnCancelClicked()
{
    this->close();
}
}
