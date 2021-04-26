/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "generator/PublicRSACryptoKeyGenerator.hpp"

namespace key_generator {
PublicRSACryptoKeyGenerator::PublicRSACryptoKeyGenerator(CryptoPP::InvertibleRSAFunction keyParams)
    : keyParams_ { std::move(keyParams) }
{
}

CryptoPP::RSA::PublicKey PublicRSACryptoKeyGenerator::getPublicKey() const
{
    CryptoPP::RSA::PublicKey publicKey;
    CryptoPP::ArraySource src { reinterpret_cast<const CryptoPP::byte *>(key_.data()), key_.size(),
                                true };
    publicKey.BERDecode(src);
    return publicKey;
}

const std::vector<std::byte> &PublicRSACryptoKeyGenerator::getGeneratedKey()
{
    return key_;
}

void PublicRSACryptoKeyGenerator::generate()
{
    std::vector<CryptoPP::byte> buffer;
    CryptoPP::RSA::PublicKey publicKey { keyParams() };
    CryptoPP::VectorSink encoder { buffer };
    publicKey.DEREncode(encoder);

    auto begBuffer = reinterpret_cast<std::byte *>(buffer.data());
    key_ = { begBuffer, begBuffer + buffer.size() };
}

const CryptoPP::InvertibleRSAFunction &PublicRSACryptoKeyGenerator::keyParams() const
{
    return keyParams_;
}

void PublicRSACryptoKeyGenerator::setKeyParams(CryptoPP::InvertibleRSAFunction keyParams)
{
    keyParams_ = std::move(keyParams);
}
}