/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "generator/PrivateRSACryptoKeyGenerator.hpp"

namespace key_generator
{
    PrivateRSACryptoKeyGenerator::PrivateRSACryptoKeyGenerator(CryptoPP::InvertibleRSAFunction keyParams):
        _keyParams(std::move(keyParams))
    {
    }

    void PrivateRSACryptoKeyGenerator::generate()
    {
        std::vector<CryptoPP::byte> buffer;
        CryptoPP::RSA::PrivateKey key{ _keyParams };
        CryptoPP::VectorSink encoder{ buffer };

        key.DEREncode(encoder);
        _key.clear();
        _key.shrink_to_fit();
        _key.reserve(buffer.size());
        std::transform(
            buffer.begin(),
            buffer.end(),
            std::back_inserter(_key),
            [](const auto &itr) { return static_cast<std::byte>(itr); }
        );
    }
    
    void PrivateRSACryptoKeyGenerator::setKeyParams(CryptoPP::InvertibleRSAFunction keyParams)
    {
        _keyParams = std::move(keyParams);
    }
    
    const CryptoPP::InvertibleRSAFunction &PrivateRSACryptoKeyGenerator::keyParams() const
    {
        return _keyParams;
    }
    
    std::vector<std::byte> PrivateRSACryptoKeyGenerator::getGeneratedKey()
    {
        return _key;
    }
}
