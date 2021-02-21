/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <aes.h>
#include <boost/assert.hpp>
#include <modes.h>
#include <osrng.h>
#include <stdexcept>

#include "codec/AESEncoderCodec.hpp"

namespace codec
{
    AESEncoderCodec::AESEncoderCodec(std::vector<std::byte> key):
        AESEncoderCodec(decltype(buffer){}, std::move(key))
    {
    }
    
    AESEncoderCodec::AESEncoderCodec(std::vector<std::byte> data, std::vector<std::byte> key):
        buffer{ std::move(data) }, key{ std::move(key) }
    {
        if (this->key.empty())
            throw std::invalid_argument("AES key must not be empty.");
    }
    
    AESEncoderCodec::AESEncoderCodec(const std::byte *data, std::size_t size, std::vector<std::byte> key):
        AESEncoderCodec(std::move(key))
    {
        if (data == nullptr)
            throw std::invalid_argument{ "Data must not be nullptr but seems to receive it." };

        buffer = decltype(buffer){ data, data + size };
    }
    
    std::vector<std::byte> AESEncoderCodec::getCodecResult()
    {
        return encoded;
    }
    
    void AESEncoderCodec::setCodecData(std::vector<std::byte> data)
    {
        buffer = std::move(data);
    }
    
    void AESEncoderCodec::execute()
    {
        CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption prndPool;
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encAes;
        CryptoPP::SecByteBlock iv{ CryptoPP::AES::BLOCKSIZE };
        CryptoPP::SecByteBlock encKey{ key.size() };
        CryptoPP::SecByteBlock prndPoolIV{ CryptoPP::AES::Encryption::BLOCKSIZE };

        std::transform(
            key.crbegin(),
            key.crbegin() + prndPoolIV.size(),
            prndPoolIV.begin(),
            [](const auto &itr) { return static_cast<CryptoPP::byte>(itr); }
        );

        prndPool.SetKeyWithIV(reinterpret_cast<CryptoPP::byte*>(key.data()), key.size(), prndPoolIV);
        prndPool.GenerateBlock(iv, iv.size());
        prndPool.GenerateBlock(encKey, encKey.size());

        encAes.SetKeyWithIV(encKey, encKey.size(), iv);

        std::vector<CryptoPP::byte> result;
        CryptoPP::ArraySource encPrc1{
            reinterpret_cast<CryptoPP::byte*>(buffer.data()),
            buffer.size(),
            true,
            new CryptoPP::StreamTransformationFilter{
                encAes,
                new CryptoPP::VectorSink{ result }
            }
        };

        encoded.clear();
        encoded.shrink_to_fit();
        encoded.reserve(result.size());
        std::transform(
            result.begin(),
            result.end(),
            std::back_inserter(encoded),
            [](const auto &itr) { return static_cast<std::byte>(itr); }
        );
    }
}