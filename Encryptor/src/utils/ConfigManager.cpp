/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <fstream>
#include <QFileInfo>
#include <utils/ConfigManager.hpp>
#include <yaml-cpp/yaml.h>

namespace utils
{
    ConfigManager &ConfigManager::getInstance()
    {
        static ConfigManager instance;
        return instance;
    }

    void ConfigManager::loadConfig()
    {
        QFileInfo cfgFileInfo{ ConfigName::cfgFileName.data() };
        if (!cfgFileInfo.exists()) return;

        YAML::Node document;
        document = YAML::LoadFile(cfgFileInfo.absoluteFilePath().toStdString());
        setEnableHighDPIScaling(document["app"][ConfigName::enableHighDPIScaling.data()].as<bool>(isEnableHighDPIScaling()));
    }

    void ConfigManager::dumpConfig()
    {
        YAML::Node document;
        
        document["app"][ConfigName::enableHighDPIScaling.data()] = isEnableHighDPIScaling();

        std::ofstream cfgWriter{ ConfigName::cfgFileName.data() };
        if (!cfgWriter.is_open()) return;
        cfgWriter << YAML::Dump(document) << std::endl;
        cfgWriter.close();
    }

    bool ConfigManager::isEnableHighDPIScaling() const
    {
        return _enableHighDPIScaling;
    }

    void ConfigManager::setEnableHighDPIScaling(bool value)
    {
        _enableHighDPIScaling = value;
    }

    ConfigManager::ConfigManager()
    {
    }
}