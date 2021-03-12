/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <cryptopp/osrng.h>

#include "codec/RSASignEncoderCodec.hpp"

namespace codec
{
    RSAEncoderCodec::RSAEncoderCodec(std::vector<std::byte> data, CryptoPP::RSA::PrivateKey key):
        _key{ std::move(key) }, _buffer{ std::move(data) }
    {
    }

    RSAEncoderCodec::RSAEncoderCodec(const std::byte *data, std::size_t size, CryptoPP::RSA::PrivateKey key):
        _key{ std::move(key) }, _buffer{ decltype(_buffer)(data, data + size) }
    {
    }
    
    RSAEncoderCodec::RSAEncoderCodec(std::string_view data, CryptoPP::RSA::PrivateKey key):
        RSAEncoderCodec(reinterpret_cast<const std::byte *>(data.data()), data.length(), std::move(key))
    {
    }
    
    std::vector<std::byte> RSAEncoderCodec::getCodecResult() const
    {
        return _encodedData;
    }

    void RSAEncoderCodec::setCodecData(std::vector<std::byte> data)
    {
        _buffer = std::move(data);
    }

    void RSAEncoderCodec::setCodecData(const std::byte *data, std::size_t size)
    {
        _buffer = decltype(_buffer)(data, data + size);
    }

    void RSAEncoderCodec::setCodecData(std::string_view data)
    {
        setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.length());
    }

    void RSAEncoderCodec::execute()
    {
        CryptoPP::AutoSeededRandomPool rndPool;
        CryptoPP::RSASSA_PKCS1v15_SHA_Signer encoder{ _key };
        std::vector<CryptoPP::byte> bufEncoded;

        static_cast<void>(CryptoPP::ArraySource{
            reinterpret_cast<CryptoPP::byte *>(_buffer.data()),
            _buffer.size(),
            true,
            new CryptoPP::SignerFilter{
                rndPool,
                encoder,
                new CryptoPP::VectorSink{ bufEncoded }
            }
        });

        auto begBufEncoded = reinterpret_cast<decltype(_encodedData)::value_type *>(bufEncoded.data());
        auto endBufEncoded = begBufEncoded + bufEncoded.size();
        setEncodedData({ begBufEncoded, endBufEncoded });
    }
    
    void RSAEncoderCodec::setKey(CryptoPP::RSA::PrivateKey key)
    {
        _key = std::move(key);
    }
    
    const CryptoPP::RSA::PrivateKey &RSAEncoderCodec::key() const
    {
        return _key;
    }
    
    void RSAEncoderCodec::setEncodedData(std::vector<std::byte> value)
    {
        _encodedData = std::move(value);
    }
}