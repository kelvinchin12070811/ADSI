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
DefaultCryptoKeyGeneratorFactory::createDefaultPublicASymEncryptionKey(
        const CryptoPP::RandomizedTrapdoorFunctionInverse &params)
{
    try {
        const auto &rsaParams = dynamic_cast<const CryptoPP::InvertibleRSAFunction &>(params);
        return std::make_unique<PublicRSACryptoKeyGenerator>(rsaParams);
    } catch (const std::bad_cast &) {
        throw std::invalid_argument { "Params function passed must be InvertibleRSAFunction." };
    }
}

std::unique_ptr<ICryptoKeyGenerator>
DefaultCryptoKeyGeneratorFactory::createDefaultPrivateASymEncryptionKey(
        const CryptoPP::RandomizedTrapdoorFunctionInverse &params)
{
    try {
        const auto &rsaParams = dynamic_cast<const CryptoPP::InvertibleRSAFunction &>(params);
        return std::make_unique<PrivateRSACryptoKeyGenerator>(rsaParams);
    } catch (const std::bad_cast &) {
        throw std::invalid_argument { "Params function passed must be InvertibleRSAFunction." };
    }
}
}