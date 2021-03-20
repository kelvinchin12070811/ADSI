/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "generator/RSACryptoKeyGeneratorBase.hpp"

namespace key_generator
{
    /**
     * @brief Public key generator of RSA algorithm.
     */
    class PublicRSACryptoKeyGenerator : public RSACryptoKeyGeneratorBase
    {
    public:
        /**
         * @brief Create public RSA key generator with @p keyParams.
         * @param keyParams Parameters use to generate key.
        */
        explicit PublicRSACryptoKeyGenerator(CryptoPP::InvertibleRSAFunction keyParams);
        /**
         * @brief Get generated public key.
         * @return RSA public key.
         */
        CryptoPP::RSA::PublicKey getPublicKey() const;
        const std::vector<std::byte> &getGeneratedKey() override;
        void generate() override;

    public: // Accessors
        /**
         * @brief Get parameters to generate public key.
         * @return Key parameters.
         * 
         * @sa setKeyParams(CryptoPP::InvertibleRSAFunction)
         */
        const CryptoPP::InvertibleRSAFunction &keyParams() const;

    public: // Mutators
        /**
         * @brief Assign new key parameters to generator.
         * @param keyParams New key parameters to assign.
         * 
         * @sa keyParams()
         */
        void setKeyParams(CryptoPP::InvertibleRSAFunction keyParams);

    private: // Private Mutators
        /**
         * @brief Assign new value to generated key buffer.
         * @param value New value to assign.
         */
        void setKey(std::vector<std::byte> value);

    private:
        /**
         * @name Attributes
         * @{
         */
        /**
         * @brief Key parameters to generate public key.
         * 
         * @sa keyParams()
         * @sa setKeyParams(CryptoPP::InvertibleRSAFunction)
         */
        CryptoPP::InvertibleRSAFunction _keyParams;
        /**
         * @brief Public key generated by the generator.
         * 
         * @sa setKey(std::vector<std::byte>)
         */
        std::vector<std::byte> _key;
        /** @} */
    };
}