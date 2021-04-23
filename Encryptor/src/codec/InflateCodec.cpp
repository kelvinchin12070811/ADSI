/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <boost/range/irange.hpp>
#include <fmt/format.h>
#include <zlib.h>

#include "codec/InflateCodec.hpp"

namespace codec {
InflateCodec::InflateCodec() { }

InflateCodec::InflateCodec(std::vector<std::byte> data) : buffer_ { std::move(data) } { }

InflateCodec::InflateCodec(std::string_view data)
    : InflateCodec(reinterpret_cast<const std::byte *>(data.data()), data.length())
{
}

InflateCodec::InflateCodec(const std::byte *data, std::size_t length)
    : buffer_ { data, data + length }
{
}

void InflateCodec::setCodecData(std::vector<std::byte> data)
{
    buffer_ = std::move(data);
}

void InflateCodec::setCodecData(std::string_view data)
{
    setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.length());
}

void InflateCodec::setCodecData(const std::byte *data, std::size_t length)
{
    buffer_ = { data, data + length };
}

void InflateCodec::execute()
{
    std::uint32_t szData { 0 };
    for (auto idx : boost::irange(sizeof(std::uint32_t)))
        reinterpret_cast<std::byte *>(&szData)[idx] = buffer_[idx];

    encoded_ = {};
    encoded_.resize(szData);
    auto szDest = static_cast<uLongf>(szData);
    auto result =
            uncompress(reinterpret_cast<Bytef *>(encoded_.data()), &szDest,
               reinterpret_cast<const Bytef *>(buffer_.data()) + sizeof(std::uint32_t),
               static_cast<uLong>(buffer_.size()));

    switch (result) {
    case Z_BUF_ERROR:
        throw std::length_error { "Buffer not large enough to hold uncompressed data." };
        break;
    case Z_MEM_ERROR:
        throw std::runtime_error { "Not enough memory allocated to the decoder." };
        break;
    case Z_DATA_ERROR:
        throw std::domain_error { "Corrupted or incomplete data." };
        break;
    }
}

const std::vector<std::byte> &InflateCodec::getCodecResult() const
{
    return encoded_;
}
}