/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstddef>
#include <vector>

namespace key_generator
{
    /**
     * @brief Interface of all Crypto key generators.
     */
    struct ICryptoKeyGenerator
    {
        /**
         * @brief Get generated key from the generator
         * @return std::vector of std::bytes represented as generated Crypto key.
         */
        virtual std::vector<std::byte> getGeneratedKey() = 0 {}
        /**
         * @brief Trigger generation of crypto key.
         */
        virtual void generate() = 0 {}
    };
}