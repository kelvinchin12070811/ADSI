/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <initializer_list>
#include <numeric>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <stdexcept>

#include "utils/ConfigManager.hpp"
#include "utils/StylesManager.hpp"
#include "window/setting/Setting.hpp"

namespace window {
Setting::Setting(QWidget *parent) : QDialog(parent), ui_ { std::make_unique<Ui::settingDialog>() }
{
    ui_->setupUi(this);

    loadStylesheet();
    loadConfigs();
}

void Setting::loadStylesheet()
{
    utils::StylesManager::getInstance().applyStylesheets(
            this, { QStringLiteral(":/Themes/Default/SettingWindow.qss") });
}

void Setting::loadConfigs()
{
    auto configMng = &utils::ConfigManager::getInstance();
    ui_->optEnableHighDPIScaling->setChecked(configMng->isEnableHighDPIScaling());
}

void Setting::onBtnCancelClicked()
{
    this->close();
}

void Setting::onBtnOkClicked()
{
    auto configMng = &utils::ConfigManager::getInstance();

    configMng->setEnableHighDPIScaling(ui_->optEnableHighDPIScaling->isChecked());

    configMng->dumpConfig();
    QMessageBox::information(this, QStringLiteral("ADSI Encryptor"),
                             tr("Settings saved, restart application to take effect."));
    this->close();
}
}