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

namespace codec {
AESDecoderCodec::AESDecoderCodec(std::vector<std::byte> key)
    : AESDecoderCodec(nullptr, 0, std::move(key))
{
}

AESDecoderCodec::AESDecoderCodec(std::vector<std::byte> data, std::vector<std::byte> key)
    : AESDecoderCodec(nullptr, 0, std::move(key))
{
    buffer_ = std::move(data);
}

AESDecoderCodec::AESDecoderCodec(std::string_view data, std::vector<std::byte> key)
    : AESDecoderCodec(reinterpret_cast<const std::byte *>(data.data()), data.length(),
                      std::move(key))
{
}

AESDecoderCodec::AESDecoderCodec(const std::byte *data, std::size_t size,
                                 std::vector<std::byte> key)
    : key_ { std::move(key) }
{
    if (data == nullptr)
        return;

    buffer_ = { data, data + size };
}

const std::vector<std::byte> &AESDecoderCodec::getCodecResult() const
{
    return encoded_;
}

void AESDecoderCodec::setCodecData(std::vector<std::byte> data)
{
    buffer_ = std::move(data);
}

void AESDecoderCodec::setCodecData(const std::byte *data, std::size_t size)
{
    if (data == nullptr)
        throw std::invalid_argument { "Parameter data must not be nullptr but seems to be." };

    buffer_ = { data, data + size };
}

void AESDecoderCodec::setCodecData(std::string_view data)
{
    setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.size());
}

void AESDecoderCodec::execute()
{
    CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption prndPool;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decAes;
    CryptoPP::SecByteBlock iv { CryptoPP::AES::BLOCKSIZE };
    CryptoPP::SecByteBlock decKey { key_.size() };
    CryptoPP::SecByteBlock prndPoolIV { CryptoPP::AES::BLOCKSIZE };

    std::transform(key_.crbegin(), key_.crbegin() + prndPoolIV.size(), prndPoolIV.begin(),
                   [](const auto &itr) { return static_cast<CryptoPP::byte>(itr); });

    prndPool.SetKeyWithIV(reinterpret_cast<CryptoPP::byte *>(key_.data()), key_.size(), prndPoolIV);
    prndPool.GenerateBlock(iv, iv.size());
    prndPool.GenerateBlock(decKey, decKey.size());

    decAes.SetKeyWithIV(decKey, decKey.size(), iv);

    std::vector<CryptoPP::byte> result;
    static_cast<void>(CryptoPP::ArraySource {
            reinterpret_cast<CryptoPP::byte *>(buffer_.data()), buffer_.size(), true,
            new CryptoPP::StreamTransformationFilter { decAes,
                                                       new CryptoPP::VectorSink { result } } });

    auto begResult = reinterpret_cast<std::byte *>(result.data());
    encoded_ = { begResult, begResult + result.size() };
}

void AESDecoderCodec::setKey(std::vector<std::byte> key)
{
    if (key.empty())
        throw std::invalid_argument { "Parameter key must not be empty but seems to be" };

    key_ = std::move(key);
}
}