/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

#include "codec/Base64EncoderCodec.hpp"

namespace codec {
Base64EncoderCodec::Base64EncoderCodec() { }

Base64EncoderCodec::Base64EncoderCodec(std::vector<std::byte> data)
    : buffer_ { std::move(data) } { }

Base64EncoderCodec::Base64EncoderCodec(std::string_view data)
    : Base64EncoderCodec(reinterpret_cast<const std::byte *>(data.data()), data.size())
{
}

Base64EncoderCodec::Base64EncoderCodec(const std::byte *data, std::size_t length)
    : buffer_ { data, data + length }
{
}

const std::vector<std::byte> &Base64EncoderCodec::getCodecResult() const
{
    return encoded_;
}

void Base64EncoderCodec::setCodecData(std::vector<std::byte> data)
{
    buffer_ = std::move(data);
}

void Base64EncoderCodec::setCodecData(std::string_view data)
{
    setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.size());
}

void Base64EncoderCodec::setCodecData(const std::byte *data, std::size_t length)
{
    buffer_ = { data, data + length };
}

void Base64EncoderCodec::execute()
{
    std::vector<CryptoPP::byte> bufExec;
    static_cast<void>(CryptoPP::ArraySource {
            reinterpret_cast<CryptoPP::byte *>(buffer_.data()), buffer_.size(), true,
            new CryptoPP::Base64Encoder { new CryptoPP::VectorSink { bufExec }, false } });
    
    auto begBufExec = reinterpret_cast<std::byte *>(bufExec.data());
    encoded_ = { begBufExec, begBufExec + bufExec.size() };
}
}