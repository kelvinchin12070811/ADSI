/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include "generator/ICryptoKeyGeneratorFactory.hpp"

namespace key_generator {
/**
 * @brief Factory that create a set of default selected codecs.
 */
class DefaultCryptoKeyGeneratorFactory : public ICryptoKeyGeneratorFactory
{
public:
    std::unique_ptr<ICryptoKeyGenerator>
    createDefaultSymEncryptionKey(std::string password) override;
    std::unique_ptr<ICryptoKeyGenerator> createDefaultPublicASymEncryptionKey(
            const CryptoPP::RandomizedTrapdoorFunctionInverse &params) override;
    std::unique_ptr<ICryptoKeyGenerator> createDefaultPrivateASymEncryptionKey(
            const CryptoPP::RandomizedTrapdoorFunctionInverse &params) override;
    std::unique_ptr<CryptoPP::RandomizedTrapdoorFunctionInverse> generateASymParams() override;
    std::string
    serializeKeyParams(const CryptoPP::RandomizedTrapdoorFunctionInverse &params) override;
    std::unique_ptr<CryptoPP::RandomizedTrapdoorFunctionInverse>
    deserializeKeyParams(std::string_view params);
};
}