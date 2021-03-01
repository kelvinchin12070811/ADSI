/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <algorithm>
#include <boost/assert.hpp>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

#include "codec/AESDecoderCodec.hpp"

namespace codec
{
    AESDecoderCodec::AESDecoderCodec(std::vector<std::byte> key):
        AESDecoderCodec(nullptr, 0, std::move(key))
    {
    }

    AESDecoderCodec::AESDecoderCodec(std::vector<std::byte> data, std::vector<std::byte> key):
        AESDecoderCodec(nullptr, 0, std::move(key))
    {
        _buffer = std::move(data);
    }

    AESDecoderCodec::AESDecoderCodec(std::string_view data, std::vector<std::byte> key):
        AESDecoderCodec(reinterpret_cast<const std::byte *>(data.data()), data.length(), std::move(key))
    {
    }

    AESDecoderCodec::AESDecoderCodec(const std::byte *data, std::size_t size, std::vector<std::byte> key):
        _key{ std::move(key) }
    {
        if (data == nullptr) return;

        _buffer.reserve(size);
        std::copy(data, data + size, std::back_inserter(_buffer));
    }
    
    std::vector<std::byte> AESDecoderCodec::getCodecResult() const
    {
        return _encoded;
    }
    
    void AESDecoderCodec::setCodecData(std::vector<std::byte> data)
    {
        _buffer = std::move(data);
    }

    void AESDecoderCodec::setCodecData(const std::byte *data, std::size_t size)
    {
        if (data == nullptr)
            throw std::invalid_argument{ "Parameter data must not be nullptr but seems to be." };

        _buffer.clear();
        _buffer.shrink_to_fit();
        _buffer.reserve(size);
        std::copy(data, data + size, std::back_inserter(_buffer));
    }

    void AESDecoderCodec::setCodecData(std::string_view data)
    {
        setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.size());
    }
    
    void AESDecoderCodec::execute()
    {
        CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption prndPool;
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decAes;
        CryptoPP::SecByteBlock iv{ CryptoPP::AES::BLOCKSIZE };
        CryptoPP::SecByteBlock decKey{ _key.size() };
        CryptoPP::SecByteBlock prndPoolIV{ CryptoPP::AES::BLOCKSIZE };

        std::transform(
            _key.crbegin(),
            _key.crbegin() + prndPoolIV.size(),
            prndPoolIV.begin(),
            [](const auto &itr) { return static_cast<CryptoPP::byte>(itr); }
        );

        prndPool.SetKeyWithIV(reinterpret_cast<CryptoPP::byte *>(_key.data()), _key.size(), prndPoolIV);
        prndPool.GenerateBlock(iv, iv.size());
        prndPool.GenerateBlock(decKey, decKey.size());

        decAes.SetKeyWithIV(decKey, decKey.size(), iv);
        
        std::vector<CryptoPP::byte> result;
        static_cast<void>(CryptoPP::ArraySource{
            reinterpret_cast<CryptoPP::byte *>(_buffer.data()),
            _buffer.size(),
            true,
            new CryptoPP::StreamTransformationFilter{
                decAes,
                new CryptoPP::VectorSink{ result }
            }
        });

        _encoded.clear();
        _encoded.shrink_to_fit();
        _encoded.reserve(result.size());
        std::transform(
            result.cbegin(),
            result.cend(),
            std::back_inserter(_encoded),
            [](const auto &itr) { return static_cast<std::byte>(itr); }
        );
    }
    
    void AESDecoderCodec::setKey(std::vector<std::byte> key)
    {
        if (key.empty())
            throw std::invalid_argument{ "Parameter key must not be empty but seems to be" };

        _key = std::move(key);
    }
}