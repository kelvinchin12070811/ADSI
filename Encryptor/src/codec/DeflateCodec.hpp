/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include "codec/ICodec.hpp"

namespace codec {
/**
 * @brief Codec that use deflate to compress data.
 */
class DeflatCodec : public ICodec
{
public:
    /**
     * @brief Create default codec without data.
     */
    DeflatCodec();
    /**
     * @brief Create codec with array of data.
     * @param data Array of data to assign to the buffer.
     */
    explicit DeflatCodec(std::vector<std::byte> data);
    /**
     * @brief Create codec with string of text.
     * @param data String to assign to the buffer.
     */
    explicit DeflatCodec(std::string_view data);
    /**
     * @brief Create codec with C style array of data.
     * @param data Pointer to first element in array.
     * @param length Length of the array.
     */
    explicit DeflatCodec(const std::byte *data, size_t length);

    void setCodecData(std::vector<std::byte> data) override;
    void setCodecData(std::string_view data) override;
    void setCodecData(const std::byte *data, size_t length) override;

    /**
     * @copydoc codec::ICodec::execute()
     * @throw std::length_error if buffer not large enough.
     * @throw std::runtime_error if not enough memory.
     */
    void execute() override;

    const std::vector<std::byte> &getCodecResult() const override;

private:
    /**
     * @brief Buffer of data to encode.
     */
    std::vector<std::byte> buffer_;
    /**
     * @brief Encoded data to get with getCodecResult()
     */
    std::vector<std::byte> encoded_;
};
}