/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstddef>
#include <memory>
#include <string_view>
#include <variant>
#include <vector>

#include "codec/ICodec.hpp"

namespace codec {
/**
 * @brief Interface of factory that control the creation of the codecs available.
 */
struct ICodecFactory
{
    /**
     * @brief Type of data supported by the codec.
     */
    using CodecDataStream = std::variant<std::vector<std::byte>, std::string_view,
                                         std::pair<const std::byte *, std::size_t>>;

    /**
     * @brief Create default binary to text encoder.
     * @param data Data to pass into the encoder.
     * @return New encoder consturcted.
     * @throw std::invalid_argument if @p data does not contain valid data or it's empty.
     */
    virtual std::unique_ptr<ICodec> createDefaultB2TEncoder(CodecDataStream data) = 0 { }
};
}