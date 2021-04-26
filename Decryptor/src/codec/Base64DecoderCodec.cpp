/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

#include "codec/Base64DecoderCodec.hpp"

namespace codec {
Base64DecoderCodec::Base64DecoderCodec(std::vector<std::byte> data)
    : buffer_ { std::move(data) } { }

Base64DecoderCodec::Base64DecoderCodec(std::string_view str)
    : Base64DecoderCodec(reinterpret_cast<const std::byte *>(str.data()), str.length())
{
}

Base64DecoderCodec::Base64DecoderCodec(const std::byte *data, std::size_t length)
    : buffer_ { data, data + length }
{
}

const std::vector<std::byte> &Base64DecoderCodec::getCodecResult() const
{
    return encoded_;
}

void Base64DecoderCodec::setCodecData(std::vector<std::byte> data)
{
    buffer_ = std::move(data);
}

void Base64DecoderCodec::setCodecData(std::string_view str)
{
    setCodecData(reinterpret_cast<const std::byte *>(str.data()), str.length());
}

void Base64DecoderCodec::setCodecData(const std::byte *data, std::size_t length)
{
    buffer_ = { data, data + length };
}

void Base64DecoderCodec::execute()
{
    std::vector<CryptoPP::byte> bufDecode;
    static_cast<void>(CryptoPP::ArraySource {
            reinterpret_cast<const CryptoPP::byte *>(buffer_.data()), buffer_.size(), true,
            new CryptoPP::Base64Decoder { new CryptoPP::VectorSink { bufDecode } } });
    
    auto begBufDecoded = reinterpret_cast<std::byte *>(bufDecode.data());
    encoded_ = { begBufDecoded, begBufDecoded + bufDecode.size() };
}
}