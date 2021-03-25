/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <boost/assert.hpp>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <stdexcept>

#include "codec/AESEncoderCodec.hpp"

namespace codec {
AESEncoderCodec::AESEncoderCodec(std::vector<std::byte> key)
    : AESEncoderCodec(decltype(_buffer) {}, std::move(key))
{
}

AESEncoderCodec::AESEncoderCodec(std::vector<std::byte> data, std::vector<std::byte> key)
    : _buffer { std::move(data) }, _key { std::move(key) }
{
    if (this->_key.empty())
        throw std::invalid_argument("AES key must not be empty.");
}

AESEncoderCodec::AESEncoderCodec(const std::byte *data, std::size_t size,
                                 std::vector<std::byte> key)
    : AESEncoderCodec(std::move(key))
{
    if (data == nullptr)
        throw std::invalid_argument { "Data must not be nullptr but seems to receive it." };

    _buffer = { data, data + size };
}

AESEncoderCodec::AESEncoderCodec(std::string_view data, std::vector<std::byte> key)
    : AESEncoderCodec(key)
{
    auto begin = reinterpret_cast<const std::byte *>(data.data());
    _buffer = { begin, begin + data.size() };
}

const std::vector<std::byte> &AESEncoderCodec::getCodecResult() const
{
    return _encoded;
}

void AESEncoderCodec::setCodecData(std::vector<std::byte> data)
{
    _buffer = std::move(data);
}

void AESEncoderCodec::setCodecData(const std::byte *data, std::size_t size)
{
    if (data == nullptr)
        throw std::invalid_argument { "Parameter data must not be nullptr but seems to be." };

    _buffer = { data, data + size };
}

void AESEncoderCodec::setCodecData(std::string_view data)
{
    setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.size());
}

void AESEncoderCodec::execute()
{
    CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption prndPool;
    CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encAes;
    CryptoPP::SecByteBlock iv { CryptoPP::AES::BLOCKSIZE };
    CryptoPP::SecByteBlock encKey { _key.size() };
    CryptoPP::SecByteBlock prndPoolIV { CryptoPP::AES::Encryption::BLOCKSIZE };

    std::transform(_key.crbegin(), _key.crbegin() + prndPoolIV.size(), prndPoolIV.begin(),
                   [](const auto &itr) { return static_cast<CryptoPP::byte>(itr); });

    prndPool.SetKeyWithIV(reinterpret_cast<CryptoPP::byte *>(_key.data()), _key.size(), prndPoolIV);
    prndPool.GenerateBlock(iv, iv.size());
    prndPool.GenerateBlock(encKey, encKey.size());

    encAes.SetKeyWithIV(encKey, encKey.size(), iv);

    std::vector<CryptoPP::byte> result;
    static_cast<void>(CryptoPP::ArraySource {
            reinterpret_cast<CryptoPP::byte *>(_buffer.data()), _buffer.size(), true,
            new CryptoPP::StreamTransformationFilter { encAes,
                                                       new CryptoPP::VectorSink { result } } });

    auto begResult = reinterpret_cast<std::byte *>(result.data());
    _encoded = { begResult, begResult + result.size() };
}

void AESEncoderCodec::setKey(std::vector<std::byte> key)
{
    key = std::move(key);
}
}