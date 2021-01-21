/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <memory>

#include <QApplication>
#include <QLabel>

#include "window/mainwindow/MainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication app{ argc, argv };

    auto window = std::make_unique<window::MainWindow>();
    window->show();

    return app.exec();
}