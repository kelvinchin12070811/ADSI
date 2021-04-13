/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include "codec/ICodecFactory.hpp"

namespace codec {
class DefaultCodecFactory : public ICodecFactory
{
public:
    std::unique_ptr<ICodec> createDefaultB2TEncoder(CodecDataStream data) override;
    std::unique_ptr<ICodec> createDefaultB2TDecoder(CodecDataStream data) override;
};
}