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
    auto lenDest = compressBound(buffer_.size());
    std::vector<Byte> dest;
    dest.resize(lenDest);
    auto result = compress(dest.data(), &lenDest, reinterpret_cast<const Bytef *>(buffer_.data()),
                           static_cast<uLong>(buffer_.size()));

    switch (result) {
    case Z_BUF_ERROR:
        throw std::length_error { "Buffer not large enough to hold compressed data." };
        break;
    case Z_MEM_ERROR:
        throw std::runtime_error { "Not enough memory allocated to the encoder." };
        break;
    }

    auto szData = static_cast<std::uint32_t>(lenDest);
    auto szOriData = static_cast<std::uint32_t>(buffer_.size());
    encoded_ = {};
    encoded_.reserve(szData + sizeof(szOriData));
    for (auto idx : boost::irange(sizeof(szOriData)))
        encoded_.emplace_back(reinterpret_cast<const std::byte *>(&szOriData)[idx]);

    std::transform(dest.begin(), dest.begin() + szData, std::back_inserter(encoded_),
                   [](const auto &item) { return static_cast<std::byte>(item); });
}

const std::vector<std::byte> &codec::DeflatCodec::getCodecResult() const
{
    return encoded_;
}
