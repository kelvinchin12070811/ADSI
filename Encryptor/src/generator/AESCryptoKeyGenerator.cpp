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

#include "AESCryptoKeyGenerator.hpp"

namespace key_generator
{
    AESCryptoKeyGenerator::AESCryptoKeyGenerator(std::string password):
        _password(password)
    {
    }

    std::vector<std::byte> AESCryptoKeyGenerator::getGeneratedKey()
    {
        return key;
    }

    void AESCryptoKeyGenerator::generate()
    {
        std::array<CryptoPP::byte, CryptoPP::SHA3_256::DIGESTSIZE> pwHash{};
        std::array<CryptoPP::byte, CryptoPP::AES::BLOCKSIZE> iv{};
        CryptoPP::SHA3_256 sha3_256Digester;
        CryptoPP::OFB_Mode<CryptoPP::AES>::Encryption psuedoRndEngine;

        std::copy(pwHash.begin(), pwHash.begin() + iv.size(), iv.begin());
        std::make_unique<CryptoPP::StringSource>(
            _password,
            true,
            new CryptoPP::HashFilter{
                sha3_256Digester,
                new CryptoPP::ArraySink{ pwHash.data(), pwHash.size() }
            }
        );

        key.resize(CryptoPP::AES::MAX_KEYLENGTH);
        psuedoRndEngine.SetKeyWithIV(pwHash.data(), pwHash.size(), pwHash.data());
        psuedoRndEngine.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(key.data()), key.size());
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