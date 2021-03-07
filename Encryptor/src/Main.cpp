/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <memory>
#include <vector>
#include <QApplication>

#include "utils/ConfigManager.hpp"
#include "window/mainwindow/MainWindow.hpp"

#if defined(WIN32) && defined(DEBUG)
#define WIN32_DEBUG
#include <Windows.h>
#endif

int main(int argc, char **argv)
{
#ifdef WIN32_DEBUG
    if (AllocConsole())
    {
        static_cast<void>(freopen("CONOUT$", "w", stdout));
        static_cast<void>(freopen("CONOUT$", "w", stderr));
    }
    system("title ADSI Encryptor Debug Console");
#endif

    utils::ConfigManager::getInstance().loadConfig();

    if (utils::ConfigManager::getInstance().isEnableHighDPIScaling())
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    auto app = std::make_unique<QApplication>(argc, argv);

    auto w = std::make_unique<window::MainWindow>();
    w->show();

    return app->exec();
}
