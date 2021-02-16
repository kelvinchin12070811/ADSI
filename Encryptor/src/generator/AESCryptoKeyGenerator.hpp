/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <string>
#include <string_view>
#include "generator/ICryptoKeyGenerator.hpp"

namespace key_generator
{
    /**
     * @brief Generator of AES Key for both encryption and decryption.
     */
    class AESCryptoKeyGenerator : public ICryptoKeyGenerator
    {
    public:
        /**
         * @brief Construct AES key generator with a password.
         * @param password Password use to generate AES key.
         */
        explicit AESCryptoKeyGenerator(std::string password);

        virtual std::vector<std::byte> getGeneratedKey() override;
        virtual void generate() override;

    public: // Assessors
        /**
         * @name Assessors
         * @{
         */

        /**
         * @brief Get password string.
         * 
         * Refer:
         *  - setPassword()
         *  - _password
         * 
         * @return std::string_view of password string.
         */
        std::string_view password() const;
        
        /** @} */

    public: // Mutators
        /**
         * @name Mutators
         * @{
         */

        /**
         * @brief Assign password string to new value
         * @param value New value to assign to password string.
         * 
         * Refer:
         *  - password
         *  - _password
         */
        void setPassword(std::string value);

        /** @} */

    private:
        std::vector<std::byte> key;

        /**
         * @name Attribute
         * @{
         */

        /**
         * @brief Password string to generate AES key.
         * 
         * Refer:
         *  - @ref setPassword()
         *  - @ref password()
         */
        std::string _password;
        /** @} */
    };
}