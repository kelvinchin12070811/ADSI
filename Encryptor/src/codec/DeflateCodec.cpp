/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <memory>
#include <stdexcept>

#include <boost/range/irange.hpp>
#include <zlib.h>

#include "codec/DeflateCodec.hpp"

codec::DeflatCodec::DeflatCodec() { }

codec::DeflatCodec::DeflatCodec(std::vector<std::byte> data) : buffer_ { data } { }

codec::DeflatCodec::DeflatCodec(std::string_view data)
    : DeflatCodec(reinterpret_cast<const std::byte *>(data.data()), data.length())
{
}

codec::DeflatCodec::DeflatCodec(const std::byte *data, size_t length)
    : buffer_ { data, data + length }
{
}

void codec::DeflatCodec::setCodecData(std::vector<std::byte> data)
{
    buffer_ = std::move(data);
}

void codec::DeflatCodec::setCodecData(std::string_view data)
{
    setCodecData(reinterpret_cast<const std::byte *>(data.data()), data.length());
}

void codec::DeflatCodec::setCodecData(const std::byte *data, size_t length)
{
    buffer_ = { data, data + length };
}

void codec::DeflatCodec::execute()
{
    std::vector<Byte> dest;
    dest.resize(buffer_.size());
    uLongf lenDest { dest.size() };
    auto result = compress(dest.data(), &lenDest, reinterpret_cast<const Bytef *>(buffer_.data()),
                           static_cast<uLong>(buffer_.size()));

    if (result != Z_OK) throw std::runtime_error { "Error on compressing data." };

    encoded_.reserve(lenDest + sizeof(std::uint32_t));
    auto szData = static_cast<std::uint32_t>(lenDest);
    auto szBytes = reinterpret_cast<std::byte *>(&szData);
    for (auto idx : boost::irange(sizeof(std::uint32_t))) encoded_.emplace_back(szBytes[idx]);

    std::move(dest.begin(), dest.begin() + lenDest, std::back_inserter(encoded_));
}

const std::vector<std::byte> &codec::DeflatCodec::getCodecResult() const
{
    return encoded_;
}
