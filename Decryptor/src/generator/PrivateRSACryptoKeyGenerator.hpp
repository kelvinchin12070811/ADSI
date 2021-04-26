/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include "generator/RSACryptoKeyGeneratorBase.hpp"

namespace key_generator {
/**
 * @brief Private key generator of RSA algorithm.
 */
class PrivateRSACryptoKeyGenerator : public RSACryptoKeyGeneratorBase
{
public:
    /**
     * @brief Construct new generator with keyParams.
     * @param keyParams Parameters to generate private key.
     */
    explicit PrivateRSACryptoKeyGenerator(CryptoPP::InvertibleRSAFunction keyParams);
    /**
     * @brief Generate private key from key data.
     * @return Private key.
     */
    CryptoPP::RSA::PrivateKey getPrivatekey() const;

    void generate() override;
    const std::vector<std::byte> &getGeneratedKey() override;

public: // Mutators
    /**
     * @brief Assign new keyParams to generator.
     * @param keyParams Parameters to assign.
     */
    void setKeyParams(CryptoPP::InvertibleRSAFunction keyParams);

public: // Accessors
    /**
     * @brief Get assigned keyParams.
     * @return Assigned keyParams.
     */
    const CryptoPP::InvertibleRSAFunction &keyParams() const;

private:
    /**
     * @name Attributes
     * @{
     */
    /**
     * @brief Key parameters to generate private key.
     *
     * @sa
     */
    CryptoPP::InvertibleRSAFunction keyParams_;
    /**
     * @brief Generated private key.
     *
     * @sa getGeneratedKey()
     */
    std::vector<std::byte> key_;
    /** @} */
};
}