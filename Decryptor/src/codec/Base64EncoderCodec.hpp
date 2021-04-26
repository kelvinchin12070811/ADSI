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
 * @brief Codec object to encode binary stream into base64 encoded text.
 */
class Base64EncoderCodec : public ICodec
{
public:
    /**
     * @brief Construct new encoder with empty data.
     */
    Base64EncoderCodec();
    /**
     * @brief Construct new encoder with array of data.
     * @param data Data to encode.
     * 
     * @sa Base64EncoderCodec(std::string_view)
     * @sa Base64EncoderCodec(const std::byte *, std::size_t)
     */
    explicit Base64EncoderCodec(std::vector<std::byte> data);
    /**
     * @brief Construct new encoder with string of data.
     * @param data String to encode
     * 
     * @sa Base64EncoderCodec(std::vector<std::byte>)
     * @sa Base64EncoderCodec(const std::byte *, std::size_t)
     */
    explicit Base64EncoderCodec(std::string_view data);
    /**
     * @brief Construct new encoder with c style array of data.
     * @param data Pointer of the beginning of the element in the array.
     * @param length Length of the data.
     * 
     * @sa Base64EncoderCodec(std::vector<std::byte>)
     * @sa Base64EncoderCodec(std::string_view)
     */
    explicit Base64EncoderCodec(const std::byte *data, std::size_t length);

    const std::vector<std::byte> &getCodecResult() const override;
    void setCodecData(std::vector<std::byte> data) override;
    void setCodecData(std::string_view data) override;
    void setCodecData(const std::byte *data, std::size_t length) override;
    void execute() override;

private:
    /**
     * @brief Data buffer that will be encoded.
     */
    std::vector<std::byte> buffer_;
    /**
     * @brief Data encoded by the encoder.
     */
    std::vector<std::byte> encoded_;
};
}