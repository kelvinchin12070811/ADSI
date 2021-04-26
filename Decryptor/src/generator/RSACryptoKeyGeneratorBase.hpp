/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cryptopp/rsa.h>

#include "generator/ICryptoKeyGenerator.hpp"

namespace key_generator {
/**
 * @brief Abstract class that RSA's generators based on.
 */
class RSACryptoKeyGeneratorBase : public ICryptoKeyGenerator
{
public:
    /**
     * @brief Generate random params for public/private keys.
     * @return Random params to generate public/private keys.
     */
    static CryptoPP::InvertibleRSAFunction generateKeyParams();
};
}