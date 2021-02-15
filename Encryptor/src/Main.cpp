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

#include "TestSubject.hpp"

int main(int argc, char **argv)
{
    TestSubject::unitTests();

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    auto app = std::make_unique<QGuiApplication>(argc, argv);
    
    QQmlApplicationEngine uiEngine;
    uiEngine.load(QStringLiteral("qrc:/src/window/mainwindow/MainWindow.qml"));

    return app->exec();
}