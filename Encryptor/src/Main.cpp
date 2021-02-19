/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <boost/range/irange.hpp>
#include <memory>
#include <vector>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#if defined(WIN32) && defined(DEBUG)
#define WIN32_DEBUG
#include <Windows.h>
#endif

#include "TestSubject.hpp"

int main(int argc, char **argv)
{
#ifdef WIN32_DEBUG
    if (AllocConsole())
    {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
    system("title ADSI Encryptor Debug Console");
#endif

    TestSubject::unitTests();

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    auto app = std::make_unique<QGuiApplication>(argc, argv);
    
    QQmlApplicationEngine uiEngine;
    uiEngine.load(QStringLiteral("qrc:/src/window/mainwindow/MainWindow.qml"));

    return app->exec();
}