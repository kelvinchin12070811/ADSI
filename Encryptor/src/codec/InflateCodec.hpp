/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstddef>
#include <vector>

#include "codec/ICodec.hpp"

namespace codec {
/**
 * @brief Codec that uncompress data encoded by deflate algorithm.
 */
class InflateCodec : public ICodec
{
public:
    /**
     * @brief Create default codec with empty buffer.
     */
    InflateCodec();
    /**
     * @brief Create codec with @p data to assign to buffer.
     * @param data Data to assign to buffer.
     */
    explicit InflateCodec(std::vector<std::byte> data);
    /**
     * @brief Create codec with @p data to assign to buffer
     * @param data String to assign to buffer.
     */
    explicit InflateCodec(std::string_view data);
    /**
     * @brief Create codec with @p data and @p length of data to assign to buffer.
     * @param data C style array of data to assign.
     * @param length Length of the data.
     */
    explicit InflateCodec(const std::byte *data, std::size_t length);

    void setCodecData(std::vector<std::byte> data) override;
    void setCodecData(std::string_view data) override;
    void setCodecData(const std::byte *data, std::size_t length) override;
    /**
     * @copydoc codec::ICodec::execute()
     * @throw std::domain_error if error on data such as incomplete/corrupted.
     * @throw std::length_error if buffer not large enough to hold uncompressed data.
     * @throw std::runtime_error if not enough memory for the codec.
     */
    void execute() override;
    const std::vector<std::byte> &getCodecResult() const override;

private:
    /**
     * @brief Buffer that holds data to uncompress.
     */
    std::vector<std::byte> buffer_;
    /**
     * @brief Array of data that been uncompressed.
     */
    std::vector<std::byte> encoded_;
};
}