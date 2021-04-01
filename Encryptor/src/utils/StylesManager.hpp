/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QString>
#include <QWidget>

#include <set>
#include <vector>

namespace utils {
/**
 * @brief Singleton object that helps to manage styles used in the ADSI Encryptor.
 */
class StylesManager
{
public:
    StylesManager(const StylesManager &) = delete;
    StylesManager(StylesManager &&) = delete;
    StylesManager &operator=(const StylesManager &) = delete;
    StylesManager &operator=(StylesManager &&) = delete;

    /**
     * @brief Get unique instance of StyleManager.
     * @return Unique instance of StyleManager.
     */
    static StylesManager &getInstance();

    /**
     * @brief Add global stylesheet which will be applied to everywhere.
     * @param path Stylesheet file path.
     */
    void addGlobalStylesheet(QString path);
    /**
     * @brief Add a set of stylesheets which will be applied to everywhere.
     * @param paths Array of paths of stylesheet.
     */
    void addGlobalStylesheets(std::vector<QString> paths);
    /**
     * @brief Apply stylesheets to @p widget.
     * @param widget Widget to apply stylesheets.
     * 
     * @sa applyStylesheets(QWidget *, std::vector<QString>)
     */
    void applyStylesheets(QWidget *widget);
    /**
     * @brief Apply stylesheets to @p widget.
     * @param widget Widget to apply stylesheets.
     * @param localStylesheetPaths Additional stylesheets to apply to @p widget only.
     * 
     * @sa applyStylesheets(QWidget *)
     */
    void applyStylesheets(QWidget *widget, std::vector<QString> localStylesheetPaths);

private:
    StylesManager();

private:
    std::set<QString> globalStylesheet_;
};
}