/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <algorithm>
#include <boost/assert.hpp>
#include <cryptopp/aes.h>

#include "codec/AESDecoderCodec.hpp"

namespace codec
{
    AESDecoderCodec::AESDecoderCodec(std::vector<std::byte> key):
        AESDecoderCodec(nullptr, 0, std::move(key))
    {
    }

    AESDecoderCodec::AESDecoderCodec(std::vector<std::byte> data, std::vector<std::byte> key):
        AESDecoderCodec(nullptr, 0, std::move(key))
    {
        _buffer = std::move(data);
    }

    AESDecoderCodec::AESDecoderCodec(std::string_view data, std::vector<std::byte> key):
        AESDecoderCodec(reinterpret_cast<const std::byte *>(data.data()), data.length(), std::move(key))
    {
    }

    AESDecoderCodec::AESDecoderCodec(const std::byte *data, std::size_t size, std::vector<std::byte> key):
        _key{ std::move(key) }
    {
        if (data == nullptr) return;

        _buffer.reserve(size);
        std::copy(data, data + size, std::back_inserter(_buffer));
    }
    
    std::vector<std::byte> AESDecoderCodec::getCodecResult() const
    {
        return _buffer;
    }
    
    void AESDecoderCodec::setCodecData(std::vector<std::byte> data)
    {
        BOOST_ASSERT_MSG(false, "unimplemented");
    }
    
    void AESDecoderCodec::execute()
    {
        BOOST_ASSERT_MSG(false, "unimplemented");
    }
    
    void AESDecoderCodec::setKey(std::vector<std::byte> key)
    {
        if (key.empty())
            throw std::invalid_argument{ "Parameter key must not be empty but seems to be" };

        _key = std::move(key);
    }

    void AESDecoderCodec::setBuffer(std::vector<std::byte> data)
    {
        _buffer = std::move(data);
    }

    void AESDecoderCodec::setBuffer(std::string_view data)
    {
        setBuffer(reinterpret_cast<const std::byte *>(data.data()), data.size());
    }
    
    void AESDecoderCodec::setBuffer(const std::byte * data, std::size_t size)
    {
        _buffer.clear();
        _buffer.shrink_to_fit();
        _buffer.reserve(size);
        std::copy(data, data + size, std::back_inserter(_buffer));
    }
}