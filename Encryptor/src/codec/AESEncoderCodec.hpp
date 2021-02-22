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
         * @sa AESEncoderCodec(const std::byte*, std::size_t)
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
         * @sa AESEncoderCodec(const std::byte*, std::size_t)
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
        AESEncoderCodec(const std::byte *data, std::size_t size, std::vector<std::byte> key);
        /**
         * @brief Construct AESEncoder with data from string
         * @param data String to encode.
         * @param key AES key for encryption.
         */
        AESEncoderCodec(std::string_view data, std::vector<std::byte> key);

        std::vector<std::byte> getCodecResult() const override;
        void setCodecData(std::vector<std::byte> data) override;
        void execute() override;

    public: // mutators
        /**
         * @name Mutators
         * @{
         */
        /**
         * @brief Set AES Key use for encryption.
         * 
         * Set AES Key use for encryption process, must not be empty.
         * 
         * @param key AES key.
         * @throw std::invalid_argument if @p key is empty.
         */
        void setKey(std::vector<std::byte> key);
        /**
         * @brief Set buffer with array of data.
         * @param data bytes to assign.
         * 
         * @sa void setBuffer(const std::byte *, std::size_t)
         * @sa void setBuffer(std::string_view)
         */
        void setBuffer(std::vector<std::byte> data);
        /**
         * @brief Assign raw array of data to buffer.
         * @param data Bytes to assign. Must not be nullptr.
         * @param size Size of data.
         * 
         * @throw std::invalid_argument if @p data is nullptr.
         * 
         * @sa void setBuffer(std::vector<std::byte>)
         * @sa void setBuffer(std::string_view)
         * 
         * @note This function will not request ownership for @p data.
         */
        void setBuffer(const std::byte *data, std::size_t size);
        /**
         * @brief Assign string to buffer.
         * @param data String to assign.
         * 
         * @sa void setBuffer(std::vector<std::byte>)
         * @sa void setBuffer(const std::byte *, std::size_t)
         * 
         * @note This function will not request ownership for @p data
         */
        void setBuffer(std::string_view data);
        /** @} */
    private:
        /**
         * @brief AES Key used in encryption process
         * 
         * @sa setKey(std::vector<std::byte>)
         */
        std::vector<std::byte> _key;
        /**
         * @brief Data buffer used to encode data.
         * 
         * @sa void setBuffer(std::vector<std::byte>)
         * @sa void setBuffer(const std::byte *, std::size_t)
         * @sa void setBuffer(std::string_view)
         */
        std::vector<std::byte> _buffer;
        /**
         * @brief Encoded data produced by the encoder.
         */
        std::vector<std::byte> _encoded;
    };
}