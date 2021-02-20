/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <vector>

#include "codec/ICodec.hpp"

namespace codec
{
    /**
     * @brief Encoder that encrypt data with AES 265 CBC algorithm.
     */
    class AESEncoderCodec : public ICodec
    {
    public:
        /**
         * @brief Construct AESEncoder with empty data stream.
         * 
         * @param key AES key for encryption, must not be empty.
         * 
         * @sa AESEncoderCodec(std::vector<std::byte>)
         * @sa AESEncoderCodec(std::byte*, std::size_t)
         * 
         * @throw std::invalid_argument if @p key is empty.
         */
        explicit AESEncoderCodec(std::vector<std::byte> key);
        /**
         * @brief Construct AESEncoder with data stream.
         * 
         * @param data Bytes to encode.
         * @param key AES key for encryption, must not be empty.
         * 
         * @sa AESEncoderCodec()
         * @sa AESEncoderCodec(std::byte*, std::size_t)
         * 
         * @throw std::invalid_argument if @p key is empty.
         */
        explicit AESEncoderCodec(std::vector<std::byte> data, std::vector<std::byte> key);
        /**
         * @brief Construct AESEncoder with raw array of data stream, must not be nullptr.
         * 
         * @param data Bytes to encode.
         * @param size Amount of bytes to process.
         * @param key AES Key for encryption, must not be empty.
         * 
         * @sa AESEncoderCodec()
         * @sa AESEncoderCodec(std::vector<std::byte>)
         * 
         * @throw std::invalid_argument if @p data is nullptr.
         * @throw std::invalid_argument if @p key is empty.
         * 
         * @note This constructor will not take ownership of @p data.
         */
        AESEncoderCodec(std::byte *data, std::size_t size, std::vector<std::byte> key);

        std::vector<std::byte> getCodecResult() override;
        void setCodecData(std::vector<std::byte> data) override;
        void execute() override;
    private:
        std::vector<std::byte> key;
        std::vector<std::byte> buffer;
        std::vector<std::byte> encoded;
    };
}