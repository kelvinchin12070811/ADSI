/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cryptopp/rsa.h>
#include "codec/ICodec.hpp"

namespace codec
{
    /**
     * @brief Codec that signing data with RSA algorithm.
     */
    class RSASignEncoderCodec : public ICodec
    {
    public:
        /**
         * @brief Create new encoder.
         * @param data Data to sign.
         * @param key Private key use to sign the data.
         * 
         * @sa RSASignEncoderCodec(const std::byte *, std::size_t, CryptoPP::RSA::PrivateKey)
         * @sa RSASignEncoderCodec(std::string_view, CryptoPP::RSA::PrivateKey)
         */
        RSASignEncoderCodec(std::vector<std::byte> data, CryptoPP::RSA::PrivateKey key);
        /**
         * @brief Create new encoder.
         * @param data Data to sign.
         * @param size Length of data.
         * @param key Private key use to sign the data.
         * 
         * @sa RSASignEncoderCodec(std::vector<std::byte>, CryptoPP::RSA::PrivateKey)
         * @sa RSASignEncoderCodec(std::string_view, CryptoPP::RSA::PrivateKey)
         */
        RSASignEncoderCodec(const std::byte *data, std::size_t size, CryptoPP::RSA::PrivateKey key);
        /**
         * @brief Create new encoder.
         * @param data Data to sign.
         * @param key Private key use to sign the data.
         * 
         * @sa RSASignEncoderCodec(std::vector<std::byte>, CryptoPP::RSA::PrivateKey)
         * @sa RSASignEncoderCodec(const std::byte *, std::size_t, CryptoPP::RSA::PrivateKey)
         */
        RSASignEncoderCodec(std::string_view data, CryptoPP::RSA::PrivateKey key);

        const std::vector<std::byte> &getCodecResult() const override;
        void setCodecData(std::vector<std::byte> data) override;
        void setCodecData(const std::byte *data, std::size_t size) override;
        void setCodecData(std::string_view data) override;
        void execute() override;

    public: // Mutators.
        /**
         * @brief Assign new key.
         * @param key Key to assign.
         */
        void setKey(CryptoPP::RSA::PrivateKey key);

    public: // Accessors
        /**
         * @brief Get current assigned private key.
         * @return Private key.
         */
        const CryptoPP::RSA::PrivateKey &key() const;

    private:
        /**
         * @brief Private key that use to sign the data.
         * 
         * @sa setKey(CryptoPP::RSA::PrivateKey key)
         * @sa key()
         */
        CryptoPP::RSA::PrivateKey _key;
        /**
         * @brief Data buffer to sign.
         */
        std::vector<std::byte> _buffer;
        /**
         * @brief Encoded data stream.
         * @sa
         */
        std::vector<std::byte> _encodedData;
    };
}