/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <string_view>

#include "codec/ICodec.hpp"

namespace {
struct ICodecFactory
{
    virtual std::unique_ptr<ICodec> createDefaultB2TEncoder(std::vector<std::byte> *data) = 0 { }
    virtual std::unique_ptr<ICodec> createDefaultB2TEncoder(std::string_view data) = 0 { }
    virtual std::unique_ptr<ICodec> createDefaultB2TEncoder(const std::byte *data,
                                                            std::size_t length) = 0
    {
    }
};
}