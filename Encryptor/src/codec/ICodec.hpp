/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstddef>
#include <string>
#include <vector>

namespace codec
{
    /**
     * @brief Base class for all Codec classes.
     */
    struct ICodec
    {
        /**
         * @brief Get result of transcoded bytestream.
         * @return Array of bytes.
         */
        virtual std::vector<std::byte> getCodecResult() const = 0 {}
        /**
         * @brief Set data for codec to transcode.
         * @param data Input data.
         */
        virtual void setCodecData(std::vector<std::byte> data) = 0 {}
        /**
         * @brief Run codec.
         * 
         * Start codec to process the data.
         */
        virtual void execute() = 0 {}
    };
}