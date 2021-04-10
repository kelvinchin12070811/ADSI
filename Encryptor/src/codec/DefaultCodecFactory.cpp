/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <stdexcept>

#include "codec/Base64EncoderCodec.hpp"
#include "codec/DefaultCodecFactory.hpp"

namespace codec {
std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultB2TEncoder(CodecDataStream data)
{
    if (data.index() == std::variant_npos) {
        throw std::invalid_argument { "Parameter data must hold data but seems to empty." };
    }

    if (std::holds_alternative<std::vector<std::byte>>(data)) {
        return std::make_unique<Base64EncoderCodec>(std::get<std::vector<std::byte>>(data));
    } else if (std::holds_alternative<std::string_view>(data)) {
        return std::make_unique<Base64EncoderCodec>(std::get<std::string_view>(data));
    } else if (std::holds_alternative<std::pair<const std::byte *, std::size_t>>(data)) {
        const auto &[pointer, size] = std::get<std::pair<const std::byte *, std::size_t>>(data);
        return std::make_unique<Base64EncoderCodec>(pointer, size);
    } else {
        return nullptr;
    }
}
}