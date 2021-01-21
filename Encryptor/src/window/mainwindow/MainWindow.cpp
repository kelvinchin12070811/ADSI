/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "MainWindow.hpp"

#include "ui_MainWindow.h"

namespace window
{
    MainWindow::MainWindow(QMainWindow* parent) :
        QMainWindow(parent), ui(std::make_unique<Ui::MainWindow>())
    {
        ui->setupUi(this);
    }
}