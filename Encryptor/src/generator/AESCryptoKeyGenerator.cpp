/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <aes.h>
#include <algorithm>
#include <array>
#include <boost/assert.hpp>
#include <boost/range/irange.hpp>
#include <memory>
#include <modes.h>
#include <osrng.h>
#include <sha3.h>

#include "codec/SHA3EncoderCodec.hpp"
#include "generator/AESCryptoKeyGenerator.hpp"

namespace key_generator
{
    AESCryptoKeyGenerator::AESCryptoKeyGenerator(std::string password):
        _password(password)
    {
    }

    std::vector<std::byte> AESCryptoKeyGenerator::getGeneratedKey()
    {
        return _key;
    }

    void AESCryptoKeyGenerator::generate()
    {
        std::vector<std::byte> pwHash{};
        std::array<CryptoPP::byte, CryptoPP::AES::BLOCKSIZE> iv{};
        CryptoPP::SHA3_256 sha3_256Digester;
        CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption psuedoRndEngine;

        auto encSHA = std::make_unique<codec::SHA3EncoderCodec>(_password);
        encSHA->execute();
        pwHash = encSHA->getCodecResult();

        std::transform(
            pwHash.crbegin(),
            pwHash.crbegin() + iv.size(),
            iv.begin(),
            [](const auto &curItem) { return static_cast<CryptoPP::byte>(curItem); }
        );

        _key.resize(CryptoPP::AES::MAX_KEYLENGTH);
        psuedoRndEngine.SetKeyWithIV(
            reinterpret_cast<CryptoPP::byte*>(pwHash.data()),
            pwHash.size(),
            iv.data()
        );
        psuedoRndEngine.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(_key.data()), _key.size());
    }

    std::string_view AESCryptoKeyGenerator::password() const
    {
        return _password;
    }
    
    void AESCryptoKeyGenerator::setPassword(std::string value)
    {
        _password = std::move(value);
    }
}