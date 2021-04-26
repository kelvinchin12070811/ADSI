/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include "codec/ICodec.hpp"

namespace codec {
/**
 * @brief Base64 decoder to decode base64 encoded data.
 */
class Base64DecoderCodec : public ICodec
{
public:
    Base64DecoderCodec() = default;
    /**
     * @brief Create decoder with array of data.
     * @param data Data to apply to buffer.
     */
    explicit Base64DecoderCodec(std::vector<std::byte> data);
    /**
     * @brief Create decoder with string of data.
     * @param str String data to apply to buffer.
     */
    explicit Base64DecoderCodec(std::string_view str);
    /**
     * @brief Create decoder with C style array of data.
     * @param data Pointer of the first data to C style array.
     * @param length Length of the data.
     */
    Base64DecoderCodec(const std::byte *data, std::size_t length);

    const std::vector<std::byte> &getCodecResult() const override;
    void setCodecData(std::vector<std::byte> data) override;
    void setCodecData(std::string_view str) override;
    void setCodecData(const std::byte *data, std::size_t length) override;
    void execute() override;

private:
    /**
     * @brief Buffer of data to encode.
     */
    std::vector<std::byte> buffer_;
    /**
     * @brief Base64 encoded data.
     */
    std::vector<std::byte> encoded_;
};
}