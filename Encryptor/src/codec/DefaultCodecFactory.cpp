/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <stdexcept>

#include "codec/Base64DecoderCodec.hpp"
#include "codec/Base64EncoderCodec.hpp"
#include "codec/DefaultCodecFactory.hpp"
#include "codec/SHA3EncoderCodec.hpp"

namespace codec {
std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultB2TEncoder(CodecDataStream data)
{
    std::unique_ptr<ICodec> codec = std::make_unique<Base64EncoderCodec>();
    setCodecBuffer(std::move(data), codec);
    return codec;
}

std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultB2TDecoder(CodecDataStream data)
{
    std::unique_ptr<ICodec> codec = std::make_unique<Base64DecoderCodec>();
    setCodecBuffer(std::move(data), codec);
    return codec;
}

std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultHashEncoder(CodecDataStream data)
{
    std::unique_ptr<ICodec> codec = std::make_unique<SHA3EncoderCodec>();
    setCodecBuffer(std::move(data), codec);
    return codec;
}

void DefaultCodecFactory::setCodecBuffer(CodecDataStream data, std::unique_ptr<ICodec> &codec)
{
    if (std::holds_alternative<ArrayDataType>(data)) {
        auto &&tmp = std::get<ArrayDataType>(data);
        codec->setCodecData(std::move(tmp));
        return;
    }

    if (std::holds_alternative<StringDataType>(data)) {
        auto &&tmp = std::get<StringDataType>(data);
        codec->setCodecData(tmp);
        return;
    }

    if (std::holds_alternative<CStyleArrayDataType>(data)) {
        const auto &[pointer, size] = std::get<CStyleArrayDataType>(data);
        codec->setCodecData(pointer, size);
        return;
    }
}
}