/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstdint>
#include <string>

namespace db::data {
struct KeyStore
{
    std::uint32_t keyID;
    std::uint32_t authorID;
    std::string keyPublic;
    std::string keyPrivate;
    std::string keyPasswordHash;
};
}