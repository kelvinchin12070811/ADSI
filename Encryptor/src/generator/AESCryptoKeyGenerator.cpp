/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <boost/assert.hpp>
#include "AESCryptoKeyGenerator.hpp"

namespace key_generator
{
    AESCryptoKeyGenerator::AESCryptoKeyGenerator(std::string password):
        _password(password)
    {
    }

    std::vector<std::byte> AESCryptoKeyGenerator::getGeneratedKey()
    {
        return key;
    }

    void AESCryptoKeyGenerator::generate()
    {
        BOOST_ASSERT_MSG(false, "unimplemented");
    }

    std::string_view AESCryptoKeyGenerator::password() const
    {
        return _password;
    }
    
    void AESCryptoKeyGenerator::setPassword(std::string value)
    {
        _password = std::move(value);
    }
}