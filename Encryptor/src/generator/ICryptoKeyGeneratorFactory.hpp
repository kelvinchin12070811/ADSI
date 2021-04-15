/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <memory>
#include <string>

#include <cryptopp/pubkey.h>

#include "generator/ICryptoKeyGenerator.hpp"

namespace key_generator {
/**
 * @brief Factory for creating different generator of key generators.
 * @note Unless specified, all generator generated via this factory and its children will not generate key by default,
 * ICryptoKeyGenerator::generate() must be called before getting the key from the generator.
 */
struct ICryptoKeyGeneratorFactory
{
    /**
     * @brief Create default key generator for default symmetric encryption.
     * @param password Password to pass into the generator.
     * @return Generator to generate key used by target default symmetric encryption.
     */
    virtual std::unique_ptr<ICryptoKeyGenerator> createDefaultSymEncryptionKey(std::string password) = 0 { }
    /**
     * @brief Create default public key generator for default asymmetric encryption.
     * @param params Parameters to generate the target type of public key.
     * @return Default public key generator.
     * @thorw std::invalid_argument if @p params type does not fit the targeted public key generator.
     */
    virtual std::unique_ptr<ICryptoKeyGenerator> createDefaultPublicASymEncryptionKey(
            const CryptoPP::RandomizedTrapdoorFunctionInverse &params) = 0
    {
    }
    /**
     * @brief Create default private key generator for default asymmetric encryption.
     * @param params Parameters to generate the target type of private key. 
     * @return Default private key generator.
     * @throw std::invalid_argument if @p params type does not fit the targeted private key generator.
     */
    virtual std::unique_ptr<ICryptoKeyGenerator> createDefaultPrivateASymEncryptionKey(
            const CryptoPP::RandomizedTrapdoorFunctionInverse &params) = 0
    {
    }
};
}