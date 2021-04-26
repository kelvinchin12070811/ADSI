/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <boost/range/irange.hpp>
#include <cryptopp/osrng.h>
#include <cryptopp/sha3.h>

#include "codec/SHA3EncoderCodec.hpp"

namespace codec {
SHA3EncoderCodec::SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3> hasher)
    : hasher_ { hasher == nullptr ? std::make_unique<CryptoPP::SHA3_256>() : std::move(hasher) }
{
}

SHA3EncoderCodec::SHA3EncoderCodec(std::vector<std::byte> data,
                                   std::unique_ptr<CryptoPP::SHA3> hasher)
    : buffer_ { std::move(data) },
      hasher_ { hasher == nullptr ? std::make_unique<CryptoPP::SHA3_256>() : std::move(hasher) }
{
}

SHA3EncoderCodec::SHA3EncoderCodec(std::string_view data, std::unique_ptr<CryptoPP::SHA3> hasher)
    : SHA3EncoderCodec(reinterpret_cast<const std::byte *>(data.data()), data.size(),
                       std::move(hasher))
{
}

SHA3EncoderCodec::SHA3EncoderCodec(const std::byte *data, std::size_t size,
                                   std::unique_ptr<CryptoPP::SHA3> hasher)
    : SHA3EncoderCodec(std::move(hasher))
{
    if (data == nullptr)
        throw std::invalid_argument { "Parameter data must not be nullptr but it seems to be" };

    buffer_ = { data, data + size };
}

const std::vector<std::byte> &SHA3EncoderCodec::getCodecResult() const
{
    return encodedData_;
}

void SHA3EncoderCodec::setCodecData(std::vector<std::byte> data)
{
    buffer_ = std::move(data);
}

void SHA3EncoderCodec::setCodecData(const std::byte *data, std::size_t size)
{
    if (data == nullptr)
        throw std::invalid_argument { "Parameter data must not be nullptr but seems to be." };

    buffer_ = { data, data + size };
}

void SHA3EncoderCodec::setCodecData(std::string_view data)
{
    setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.size());
}

void SHA3EncoderCodec::execute()
{
    encodedData_ = {};
    encodedData_.resize(hasher_->DigestSize());
    static_cast<void>(CryptoPP::ArraySource {
            reinterpret_cast<CryptoPP::byte *>(buffer_.data()), buffer_.size(), true,
            new CryptoPP::HashFilter { *hasher_.get(),
                                       new CryptoPP::ArraySink { reinterpret_cast<CryptoPP::byte *>(
                                                                         encodedData_.data()),
                                                                 encodedData_.size() } } });
}

const CryptoPP::SHA3 *SHA3EncoderCodec::hasher() const
{
    return hasher_.get();
}

void SHA3EncoderCodec::setHasher(std::unique_ptr<CryptoPP::SHA3> hasher)
{
    hasher_ = std::move(hasher);
}
}