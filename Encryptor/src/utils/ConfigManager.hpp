/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <string_view>

namespace utils
{
    /**
     * @brief Application configuration manager.
     * 
     * This singleton object manage the applications configurations.
     */
    class ConfigManager
    {
        /**
         * @brief Defines a set of name of configuration files to serialize.
         */
        struct ConfigName
        {
            /**
             * @brief Application configuration file name.
             */
            static constexpr std::string_view cfgFileName{ "app-config.yaml" };
            /**
             * @brief Name of enable high DPI scaling in config file.
             */
            static constexpr std::string_view enableHighDPIScaling{ "enable high dpi scaling" };
        };
    public:
        /**
         * @brief Get singleton instance of ConfigManager
         * 
         * Main API to access the manager.
         * 
         * @return Unique instance of ConfigManager
         */
        static ConfigManager &getInstance();
        /**
         * @brief Load configuration from file.
         */
        void loadConfig();
        /**
         * @brief Dump configuration to file.
         */
        void dumpConfig();
    public: // Accessors
        /**
         * @brief Determine if the application should scale on higher DPI displays.
         * @return True if application should scale, false otherwise.
         * 
         * @sa setEnableHighDPIScaling(bool)
         */
        bool isEnableHighDPIScaling() const;
    public: // Mutators
        /**
         * @brief Modifiy the behaviour of the application if it should scale on higher DPI display.
         * @param value New value to apply, true if the application should scale, false otherwise.
         */
        void setEnableHighDPIScaling(bool value);
    private:
        /**
         * @brief Construct new instance of ConfigManager, internal use only
        */
        ConfigManager();
        /**
         * @name Attributes
         * @{
         */
        /**
         * @brief Determine if the application should scale on high DPI display.
         * 
         * @sa isEnableHighDPIScaling()
         * @sa setEnableHighDPIScaling(bool)
         */
        bool _enableHighDPIScaling{ true };
        /** @} */
    };
}