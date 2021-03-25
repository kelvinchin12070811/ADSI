/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <algorithm>
#include <array>
#include <boost/assert.hpp>
#include <boost/range/irange.hpp>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha3.h>
#include <memory>

#include "codec/SHA3EncoderCodec.hpp"
#include "generator/AESCryptoKeyGenerator.hpp"

namespace key_generator {
AESCryptoKeyGenerator::AESCryptoKeyGenerator(std::string password) : password_(password) { }

const std::vector<std::byte> &AESCryptoKeyGenerator::getGeneratedKey()
{
    return key_;
}

void AESCryptoKeyGenerator::generate()
{
    std::vector<std::byte> pwHash {};
    std::array<CryptoPP::byte, CryptoPP::AES::BLOCKSIZE> iv {};
    CryptoPP::SHA3_256 sha3_256Digester;
    CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption psuedoRndEngine;

    auto encSHA = std::make_unique<codec::SHA3EncoderCodec>(password_);
    encSHA->execute();
    pwHash = encSHA->getCodecResult();

    std::transform(pwHash.crbegin(), pwHash.crbegin() + iv.size(), iv.begin(),
                   [](const auto &curItem) { return static_cast<CryptoPP::byte>(curItem); });

    key_.resize(CryptoPP::AES::MAX_KEYLENGTH);
    psuedoRndEngine.SetKeyWithIV(reinterpret_cast<CryptoPP::byte *>(pwHash.data()), pwHash.size(),
                                 iv.data());
    psuedoRndEngine.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(key_.data()), key_.size());
}

std::string_view AESCryptoKeyGenerator::password() const
{
    return password_;
}

void AESCryptoKeyGenerator::setPassword(std::string value)
{
    password_ = std::move(value);
}
}