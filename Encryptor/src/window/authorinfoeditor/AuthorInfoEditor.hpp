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

private:
    std::unique_ptr<Ui::AuthorInfoEditor> ui_ { nullptr };
};
}