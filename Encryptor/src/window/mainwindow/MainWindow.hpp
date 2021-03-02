/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <memory>
#include <QMainWindow>

#include "ui_MainWindow.h"

namespace window
{
    /**
     * @brief Main application window
     */
    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        /**
         * @brief Construct default window.
         * @param parent Parent of the window, nullptr for no parent.
        */
        explicit MainWindow(QWidget *parent = nullptr);

    private:
        std::unique_ptr<Ui::MainWindow> ui;
    };
}