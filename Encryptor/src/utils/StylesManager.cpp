/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QFile>
#include <QFileInfo>

#include <numeric>

#include "utils/StylesManager.hpp"

namespace utils {
StylesManager &StylesManager::getInstance()
{
    static StylesManager instance;
    return instance;
}

void StylesManager::addGlobalStylesheet(QString path)
{
    globalStylesheet_.insert(path);
}

void StylesManager::addGlobalStylesheets(std::vector<QString> paths)
{
    globalStylesheet_.insert(std::make_move_iterator(paths.begin()),
                             std::make_move_iterator(paths.end()));
}

void StylesManager::applyStylesheets(QWidget *widget)
{
    applyStylesheets(widget, {});
}

void StylesManager::applyStylesheets(QWidget *widget, std::vector<QString> localStylesheetPaths)
{
    auto stylesheet = std::accumulate(globalStylesheet_.begin(), globalStylesheet_.end(),
                                      QStringLiteral(""), [](const auto &doc, const auto &path) {
                                          if (!QFileInfo::exists(path)) return doc;
                                          QFile stylesheet { path };
                                          if (!stylesheet.open(QIODevice::OpenModeFlag::ReadOnly))
                                              return doc;
                                          QString buffer { stylesheet.readAll() };
                                          return QStringLiteral("%1\n%2").arg(doc, buffer);
                                      });
    stylesheet = std::accumulate(localStylesheetPaths.begin(), localStylesheetPaths.end(),
                                 stylesheet, [](const auto &doc, const auto &path) {
                                     if (!QFileInfo::exists(path)) return path;
                                     QFile stylesheet { path };
                                     if (!stylesheet.open(QIODevice::OpenModeFlag::ReadOnly))
                                         return doc;
                                     QString buffer { stylesheet.readAll() };
                                     return QStringLiteral("%1\n%2").arg(doc, buffer);
                                 });
    widget->setStyleSheet(stylesheet);
}

StylesManager::StylesManager() { }
}