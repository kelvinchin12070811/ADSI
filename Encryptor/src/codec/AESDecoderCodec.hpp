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
     * @brief Decoder that decode byte stgream with AES 256 CBC
     */
    class AESDecoderCodec : public ICodec
    {
    public:
        /**
         * @brief Create decryptor with empty buffer.
         * 
         * Create decryptor with no content in buffer
         * 
         * @param key AES key for decryption, must not be empty.
         * 
         * @throw std::invalid_argument if @p key is empty.
         * 
         * @sa AESDecoderCodec(std::vector<std::byte>, std::vector<std::byte>)
         * @sa AESDecoderCodec(std::string_view, std::vector<std::byte>)
         * @sa AESDecoderCodec(const std::byte *, std::size_t, std::vector<std::byte>)
         */
        AESDecoderCodec(std::vector<std::byte> key);
        /**
         * @brief Create decryptor with array of bytes.
         * 
         * Create decytptor with buffer filled with std::vector of bytes.
         * 
         * @param data Array of bytes to assign.
         * @param key AES key for decryption, must not be empty.
         * 
         * @throw std::invalid_argument if @p key is empty
         * 
         * @sa AESDecoderCodec(std::vector<std::byte>)
         * @sa AESDecoderCodec(std::string_view, std::vector<std::byte>)
         * @sa AESDecoderCodec(const std::byte *, std::size_t, std::vector<std::byte>)
         */
        AESDecoderCodec(std::vector<std::byte> data, std::vector<std::byte> key);
        /**
         * @brief Create decryptor with string.
         * @param data String to assign to buffer.
         * @param key AES key for decryption, must not be empty.
         * 
         * @throw std::invalid_argument if data is nullptr.
         * @throw std::invalid_argument if @p key is empty.
         * 
         * @sa AESDecoderCodec(std::vector<std::byte>)
         * @sa AESDecoderCodec(std::vector<std::byte>, std::vector<std::byte>)
         * @sa AESDecoderCodec(const std::byte *, std::size_t, std::vector<std::byte>)
         */
        AESDecoderCodec(std::string_view data, std::vector<std::byte> key);
        /**
         * @brief Create decryptor with data from any array of bytes.
         * @param data Bytes to decrypt, must not be nullptr.
         * @param size Amount of data to decrypt.
         * @param key AES key used to decrypt data, must not be empty.
         * 
         * @throw std::invalid_argument if @p key is empty.
         * 
         * @sa AESDecoderCodec(std::vector<std::byte>)
         * @sa AESDecoderCodec(std::vector<std::byte>, std::vector<std::byte>)
         * @sa AESDecoderCodec(std::string_view, std::vector<std::byte>)
         */
        AESDecoderCodec(const std::byte *data, std::size_t size, std::vector<std::byte> key);

        std::vector<std::byte> getCodecResult() const override;
        void setCodecData(std::vector<std::byte> data) override;
        void setCodecData(const std::byte *data, std::size_t size) override;
        void setCodecData(std::string_view data) override;
        void execute() override;

    public:// Mutators
        /**
         * @name Mutators
         * @{
         */
        /**
         * @brief Assign decryption key for decryption.
         * @param key AES key for decryption, must not be empty.
         * 
         * @throw std::invalid_argument if @param key is empty.
         */
        void setKey(std::vector<std::byte> key);
        /** @} */
    private:
        /**
         * @brief Key to use in decryption of data.
         */
        std::vector<std::byte> _key;
        /**
         * @brief Encoded data produced by decryptor.
         */
        std::vector<std::byte> _encoded;
        /**
         * @brief Buffer of data stream to encode.
         * 
         * Buffer where decryptor read and decode bytes from.
         */
        std::vector<std::byte> _buffer;
    };
}