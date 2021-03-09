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
#include "window/setting/Setting.hpp"

namespace window
{
    Setting::Setting(QWidget *parent):
        QDialog(parent), _ui{ std::make_unique<Ui::settingDialog>() }
    {
        _ui->setupUi(this);

        loadStylesheet();
        loadConfigs();
    }
    
    void Setting::loadStylesheet()
    {
        auto styles = {
            QStringLiteral(":/Themes/Default/Master.qss"),
            QStringLiteral(":/Themes/Default/SettingWindow.qss")
        };
        QString constructedStylesheet;

        constructedStylesheet = std::accumulate(
            styles.begin(),
            styles.end(),
            constructedStylesheet,
            [](const QString &prev, const QString &cur) {
                QFile input{ cur };
                input.open(QIODevice::ReadOnly);
                if (!input.isOpen()) return prev;

                const auto buf = input.readAll();
                return QStringLiteral("%1\n%2").arg(prev).arg(QString{ buf });
            }
        );

        this->setStyleSheet(constructedStylesheet);
    }

    void Setting::loadConfigs()
    {
        auto configMng = &utils::ConfigManager::getInstance();
        _ui->optEnableHighDPIScaling->setChecked(configMng->isEnableHighDPIScaling());
    }
    
    void Setting::onBtnCancelClicked()
    {
        this->close();
    }

    void Setting::onBtnOkClicked()
    {
        auto configMng = &utils::ConfigManager::getInstance();

        configMng->setEnableHighDPIScaling(_ui->optEnableHighDPIScaling->isChecked());

        configMng->dumpConfig();
        QMessageBox::information(
            this,
            QStringLiteral("ADSI Encryptor"),
            tr("Settings saved, restart application to take effect.")
        );
        this->close();
    }
}