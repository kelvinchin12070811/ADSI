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

#include "utils/DCT.hpp"

int main(int argc, char **argv)
{
    std::vector<std::vector<float>> data{{
        { 52.0f, 55.0f, 61.0f, 66.0f, 70.0f, 61.0f, 64.0f, 73.0f },
        { 63.0f, 59.0f, 55.0f, 90.0f, 109.0f, 85.0f, 69.0f, 72.0f },
        { 62.0f, 59.0f, 68.0f, 113.0f, 144.0f, 104.0f, 66.0f, 73.0f },
        { 63.0f, 58.0f, 71.0f, 122.0f, 154.0f, 106.0f, 70.0f, 69.0f },
        { 67.0f, 61.0f, 68.0f, 104.0f, 126.0f, 88.0f, 68.0f, 70.0f },
        { 79.0f, 65.0f, 60.0f, 70.0f, 77.0f, 68.0f, 58.0f, 75.0f },
        { 85.0f, 71.0f, 64.0f, 59.0f, 55.0f, 61.0f, 65.0f, 83.0f },
        { 87.0f, 79.0f, 69.0f, 68.0f, 65.0f, 76.0f, 78.0f, 94.0f }
    }};

    qDebug() << QStringLiteral("Original Data");
    for (auto &row : data)
    {
        QString output;
        for (auto &col : row)
            output += QString::number(col) + QStringLiteral(", ");

        qDebug() << output;
    }

    utils::DCT dctTransform;

    std::vector<std::vector<float>> result{ dctTransform.transfrom(data) };

    qDebug() << "DCT Encode Test";
    for (auto &row : result)
    {
        QString output;
        for (auto &col : row)
            output += QString::number(col) + QStringLiteral(", ");

        qDebug() << output;
    }

    qDebug() << "Inverse DCT Encode Test";
    auto iresult = dctTransform.itransform(result);
    for (auto &row : iresult)
    {
        QString output;
        for (auto &col : row)
            output += QString::number(col) + QStringLiteral(", ");

        qDebug() << output;
    }

    qDebug() << QStringLiteral("Data matched: ") +
        (std::mismatch(data.begin(), data.end(), iresult.begin(), iresult.end())
            .first != data.begin() ? QStringLiteral("passed") : QStringLiteral("failed"));

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    auto app = std::make_unique<QGuiApplication>(argc, argv);
    
    QQmlApplicationEngine uiEngine;
    uiEngine.load(QStringLiteral("qrc:/src/window/mainwindow/MainWindow.qml"));

    return app->exec();
}