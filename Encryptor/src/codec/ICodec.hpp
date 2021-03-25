/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace codec {
/**
 * @brief Base class for all Codec classes.
 */
struct ICodec
{
    /**
     * @brief Virtual destructor of ICodec.
     */
    virtual ~ICodec() = 0;
    /**
     * @brief Get result of transcoded bytestream.
     * @return Array of bytes.
     */
    virtual const std::vector<std::byte> &getCodecResult() const = 0 { }
    /**
     * @brief Set data for codec to transcode.
     * @param data Input data.
     *
     * @sa setCodecData(std::string_view)
     * @sa setCodecData(const std::byte *, std::size_t)
     */
    virtual void setCodecData(std::vector<std::byte> data) = 0 { }
    /**
     * @brief Set string as data for codec to transcode.
     * @param data String to transcode.
     *
     * @sa setCodecData(std::vector<std::byte>)
     * @sa setCodecData(const std::byte *, std::size_t)
     */
    virtual void setCodecData(std::string_view data) = 0 { }
    /**
     * @brief Set any array of bytes as data to transcode.
     * @param data Array of bytes. Must not be nullptr.
     * @param size Amount of bytes to consume.
     *
     * @throw std::invalid_argument if @p data is nullptr.
     *
     * @sa setCodecData(std::vector<std::byte>)
     * @sa setCodecData(std::string_view)
     */
    virtual void setCodecData(const std::byte *data, std::size_t size) = 0 { }
    /**
     * @brief Run codec.
     *
     * Start codec to process the data.
     */
    virtual void execute() = 0 { }
};

inline ICodec::~ICodec() = default;
}