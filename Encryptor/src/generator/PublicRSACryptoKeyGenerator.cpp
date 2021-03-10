/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include "generator/PublicRSACryptoKeyGenerator.hpp"

namespace key_generator
{
    PublicRSACryptoKeyGenerator::PublicRSACryptoKeyGenerator(CryptoPP::InvertibleRSAFunction keyParams):
        _keyParams{ std::move(keyParams) }
    {
    }

    CryptoPP::RSA::PublicKey PublicRSACryptoKeyGenerator::getPublicKey() const
    {
        CryptoPP::RSA::PublicKey publicKey;
        CryptoPP::ArraySource src{ reinterpret_cast<const CryptoPP::byte *>(_key.data()), _key.size(), true };
        publicKey.BERDecode(src);
        return publicKey;
    }

    std::vector<std::byte> PublicRSACryptoKeyGenerator::getGeneratedKey()
    {
        return _key;
    }

    void PublicRSACryptoKeyGenerator::generate()
    {
        std::vector<CryptoPP::byte> buffer;
        CryptoPP::RSA::PublicKey publicKey{ keyParams() };
        CryptoPP::VectorSink encoder{ buffer };
        publicKey.DEREncode(encoder);
        
        auto begin = reinterpret_cast<std::byte *>(buffer.data());
        auto end = begin + buffer.size();
        setKey(decltype(_key)(begin, end));
    }
    
    const CryptoPP::InvertibleRSAFunction &PublicRSACryptoKeyGenerator::keyParams() const
    {
        return _keyParams;
    }
    
    void PublicRSACryptoKeyGenerator::setKeyParams(CryptoPP::InvertibleRSAFunction keyParams)
    {
        _keyParams = std::move(keyParams);
    }
    
    void PublicRSACryptoKeyGenerator::setKey(std::vector<std::byte> value)
    {
        _key.swap(value);
    }
}