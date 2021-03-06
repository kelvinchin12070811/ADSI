/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "generator/PrivateRSACryptoKeyGenerator.hpp"

namespace key_generator {
PrivateRSACryptoKeyGenerator::PrivateRSACryptoKeyGenerator(
        CryptoPP::InvertibleRSAFunction keyParams)
    : keyParams_ { std::move(keyParams) }
{
}

CryptoPP::RSA::PrivateKey PrivateRSACryptoKeyGenerator::getPrivatekey() const
{
    CryptoPP::RSA::PrivateKey privateKey;
    CryptoPP::ArraySource src { reinterpret_cast<const CryptoPP::byte *>(key_.data()), key_.size(),
                                true };
    privateKey.BERDecode(src);
    return privateKey;
}

void PrivateRSACryptoKeyGenerator::generate()
{
    std::vector<CryptoPP::byte> buffer;
    CryptoPP::RSA::PrivateKey key { keyParams_ };
    CryptoPP::VectorSink encoder { buffer };

    key.DEREncode(encoder);
    auto begBuffer = reinterpret_cast<std::byte *>(buffer.data());
    key_ = { begBuffer, begBuffer + buffer.size() };
}

void PrivateRSACryptoKeyGenerator::setKeyParams(CryptoPP::InvertibleRSAFunction keyParams)
{
    keyParams_ = std::move(keyParams);
}

const CryptoPP::InvertibleRSAFunction &PrivateRSACryptoKeyGenerator::keyParams() const
{
    return keyParams_;
}

const std::vector<std::byte> &PrivateRSACryptoKeyGenerator::getGeneratedKey()
{
    return key_;
}
}
