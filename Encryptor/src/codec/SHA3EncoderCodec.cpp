/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <boost/range/irange.hpp>
#include <osrng.h>
#include <sha3.h>

#include "codec/SHA3EncoderCodec.hpp"

namespace codec
{
    SHA3EncoderCodec::SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3> hasher):
        _hasher{ hasher == nullptr ? std::make_unique<CryptoPP::SHA3_256>() : std::move(hasher) }
    {
    }

    SHA3EncoderCodec::SHA3EncoderCodec(std::vector<std::byte> data, std::unique_ptr<CryptoPP::SHA3> hasher):
        buffer(std::move(data)), _hasher(std::move(hasher))
    {
    }

    SHA3EncoderCodec::SHA3EncoderCodec(std::string_view data, std::unique_ptr<CryptoPP::SHA3> hasher):
        SHA3EncoderCodec(std::move(hasher))
    {
        buffer.clear();
        buffer.shrink_to_fit();
        buffer.reserve(data.size());

        for (const auto &itr : data)
            buffer.push_back(static_cast<std::byte>(itr));
    }

    SHA3EncoderCodec::SHA3EncoderCodec(std::byte *data, std::size_t size, std::unique_ptr<CryptoPP::SHA3> hasher):
        SHA3EncoderCodec(std::move(hasher))
    {
        buffer.clear();
        buffer.shrink_to_fit();
        buffer.reserve(size);

        for (const auto &idx : boost::irange(size))
            buffer.push_back(data[idx]);
    }

    std::vector<std::byte> SHA3EncoderCodec::getCodecResult()
    {
        return encodedData;
    }
    
    void SHA3EncoderCodec::setCodecData(std::vector<std::byte> data)
    {
        buffer = std::move(data);
    }

    void SHA3EncoderCodec::execute()
    {
        encodedData.resize(_hasher->DigestSize());
        std::make_unique<CryptoPP::ArraySource>(
            reinterpret_cast<CryptoPP::byte*>(buffer.data()),
            buffer.size(),
            true,
            new CryptoPP::HashFilter{
                *_hasher.get(),
                new CryptoPP::ArraySink{
                    reinterpret_cast<CryptoPP::byte*>(encodedData.data()),
                    encodedData.size()
                }
            }
        );
    }

    const CryptoPP::SHA3 *SHA3EncoderCodec::hasher() const
    {
        return _hasher.get();
    }
}