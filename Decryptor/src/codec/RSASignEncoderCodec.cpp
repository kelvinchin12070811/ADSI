/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <cryptopp/osrng.h>

#include "codec/RSASignEncoderCodec.hpp"

namespace codec {
RSASignEncoderCodec::RSASignEncoderCodec(std::vector<std::byte> data, CryptoPP::RSA::PrivateKey key)
    : key_ { std::move(key) }, buffer_ { std::move(data) }
{
}

RSASignEncoderCodec::RSASignEncoderCodec(const std::byte *data, std::size_t size,
                                         CryptoPP::RSA::PrivateKey key)
    : key_ { std::move(key) }, buffer_ { decltype(buffer_)(data, data + size) }
{
}

RSASignEncoderCodec::RSASignEncoderCodec(std::string_view data, CryptoPP::RSA::PrivateKey key)
    : RSASignEncoderCodec(reinterpret_cast<const std::byte *>(data.data()), data.length(),
                          std::move(key))
{
}

const std::vector<std::byte> &RSASignEncoderCodec::getCodecResult() const
{
    return encodedData_;
}

void RSASignEncoderCodec::setCodecData(std::vector<std::byte> data)
{
    buffer_ = std::move(data);
}

void RSASignEncoderCodec::setCodecData(const std::byte *data, std::size_t size)
{
    buffer_ = { data, data + size };
}

void RSASignEncoderCodec::setCodecData(std::string_view data)
{
    setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.length());
}

void RSASignEncoderCodec::execute()
{
    CryptoPP::AutoSeededRandomPool rndPool;
    CryptoPP::RSASSA_PKCS1v15_SHA_Signer signer { key() };
    auto signature = std::make_unique<std::byte[]>(signer.MaxSignatureLength());
    auto size =
            signer.SignMessage(rndPool, reinterpret_cast<const CryptoPP::byte *>(buffer_.data()),
                               buffer_.size(), reinterpret_cast<CryptoPP::byte *>(signature.get()));
    encodedData_ = { signature.get(), signature.get() + size };
}

void RSASignEncoderCodec::setKey(CryptoPP::RSA::PrivateKey key)
{
    key_ = std::move(key);
}

const CryptoPP::RSA::PrivateKey &RSASignEncoderCodec::key() const
{
    return key_;
}
}