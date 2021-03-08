/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <memory>
#include <QDialog>

#include "ui_Setting.h"

namespace window
{
    /**
     * @brief Dialog used to set configuration of the application.
     */
    class Setting : public QDialog
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct new object of Setting dialog.
         * @param parent Parent of the dialog.
         */
        explicit Setting(QWidget *parent = nullptr);
        /**
         * @brief Load stylesheet from resources.
         */
        void loadStylesheet();
    private slots:
        /**
         * @brief Action on cancel button clicked.
         */
        void onBtnCancelClicked();
        /**
         * @brief Action on ok button clicked.
         */
        void onBtnOkClicked();
    private:
        /**
         * @name Attributes
         * @{
         */
        /**
         * @brief Ui objects generated via Qt Designer.
         */
        std::unique_ptr<Ui::settingDialog> ui;
        /** @} */
    };
}