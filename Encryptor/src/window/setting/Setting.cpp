/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <initializer_list>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <stdexcept>

#include "utils/ConfigManager.hpp"
#include "window/setting/Setting.hpp"

namespace window
{
    Setting::Setting(QWidget *parent):
        QDialog(parent), ui{ std::make_unique<Ui::settingDialog>() }
    {
        ui->setupUi(this);

        loadStylesheet();
    }
    
    void Setting::loadStylesheet()
    {
        auto styles = {
            QStringLiteral(":/Themes/Default/Master.qss"),
            QStringLiteral(":/Themes/Default/SettingWindow.qss")
        };
        QString constructedStylesheet;

        for (const auto &itr : styles)
        {
            QFile file{ itr };
            file.open(QIODevice::ReadOnly);
            if (!file.isOpen())
            {
                qDebug() << QStringLiteral("Unable to load ui '%1'").arg(itr);
                continue;
            }

            constructedStylesheet += file.readAll();
        }

        this->setStyleSheet(constructedStylesheet);
    }
    
    void Setting::onBtnCancelClicked()
    {
        this->close();
    }

    void Setting::onBtnOkClicked()
    {
        utils::ConfigManager::getInstance().dumpConfig();
        QMessageBox::information(
            this,
            QStringLiteral("ADSI Encryptor"),
            tr("Settings saved, restart application to take effect.")
        );
        this->close();
    }
}