/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "generator/AESCryptoKeyGenerator.hpp"
#include "generator/PublicRSACryptoKeyGenerator.hpp"
#include "generator/PrivateRSACryptoKeyGenerator.hpp"
#include "generator/DefaultCryptoKeyGeneratorFactory.hpp"

namespace key_generator {
std::unique_ptr<ICryptoKeyGenerator>
DefaultCryptoKeyGeneratorFactory::createDefaultSymEncryptionKey(std::string password)
{
    return std::make_unique<AESCryptoKeyGenerator>(std::move(password));
}

std::unique_ptr<ICryptoKeyGenerator>
DefaultCryptoKeyGeneratorFactory::createDefaultPublicASymEncryptionKey()
{
    return std::make_unique<PublicRSACryptoKeyGenerator>();
}

std::unique_ptr<ICryptoKeyGenerator>
DefaultCryptoKeyGeneratorFactory::createDefaultPrivateASymEncryptionKey()
{
    return std::make_unique<PrivateRSACryptoKeyGenerator>();
}
}