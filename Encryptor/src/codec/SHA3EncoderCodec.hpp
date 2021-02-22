/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <cstddef>
#include <memory>
#include <sha3.h>
#include <vector>

#include <codec/ICodec.hpp>

namespace codec
{
    /**
     * @brief SHA3 encoder class.
     *
     * An encoder which take a stream of bytes and digest it into fixed length hash.
     */
    class SHA3EncoderCodec : public ICodec
    {
    public:
        /**
         * @brief Construct a new encoder with empty bytes.
         * @param hasher SHA3 instance to use, nullptr for default hash instance.
         * 
         * @sa SHA3EncoderCodec(std::vector<std::byte>, std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::string_view, std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::byte*, std::size_t, std::unique_ptr<CryptoPP::SHA3> hasher)
         */
        explicit SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3> hasher);
        /**
         * @brief Construct a new encoder with raw bytes.
         * @param data Data to encode.
         * @param hasher Hasher instance to use, nullptr for default hash instance.
         * 
         * @sa SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::string_view, std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::byte*, std::size_t, std::unique_ptr<CryptoPP::SHA3> hasher)
         */
        explicit SHA3EncoderCodec(std::vector<std::byte> data, std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);
        /**
         * @brief Construct a new encoder with encoder with bytes from std::string.
         * 
         * @param data Data to encode.
         * @param hasher Hasher instance to use, nullptr for default hash instance.
         * 
         * @sa SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::vector<std::byte>, std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::byte*, std::size_t, std::unique_ptr<CryptoPP::SHA3> hasher)
         * 
         * @note This function does not take ownership of the data.
         */
        explicit SHA3EncoderCodec(std::string_view data, std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);
        /**
         * @brief Construct a new encoder with encoder with bytes from any continues container.
         * 
         * @param data Data to encode, must not be nullptr.
         * @param size Size of data.
         * @param hasher Hasher instance to use, nullptr for default hash instance.
         * 
         * @sa SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::vector<std::byte>, std::unique_ptr<CryptoPP::SHA3>)
         * @sa SHA3EncoderCodec(std::string_view, std::unique_ptr<CryptoPP::SHA3>)
         * 
         * @note This function does not take ownership of the data.
         */
        explicit SHA3EncoderCodec(std::byte *data, std::size_t size, std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);

        std::vector<std::byte> getCodecResult() override;
        void setCodecData(std::vector<std::byte> data) override;
        void execute() override;

    public: // accessors
        /**
         * @name Accessors
         * @{
         */

        /**
         * @brief Get SHA3 hasher instance used in Encoder.
         * @return Observer pointer to SHA3 hasher instance used internally.
         */
        const CryptoPP::SHA3 *hasher() const;

        /** @} */

    public: // mutators
        /**
         * @name Mutators
         * @{
         */
        /**
         * @brief Set data buffer with array of bytes.
         * @param data Byte array to assign to buffer.
         */
        void setBuffer(std::vector<std::byte> data);
        /**
         * @brief Set data buffer with string data.
         * 
         * This function assign a string to data buffer.
         * 
         * @param data String to encode
        */
        void setBuffer(std::string_view data);
        /**
         * @brief Assign data buffer with any array container.
         * @param data Observer pointer to array of data to assign.
         * @param size Amount of data to assign.
         */
        void setBuffer(const std::byte *data, std::size_t size);
        /**
         * @brief Set SHA3 implementation used by the encoder.
         * 
         * Set the instance of SHA3 implementation used by teh encoder, nullptr for default.
         * 
         * @param hasher SHA3 instance to use.
         */
        void setHasher(std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);
        /** @} */
    private:
        /**
         * @brief SHA3 hasher instance used in hashing data.
         * 
         * The CryptoPP SHA3 hasher instance used in hashing data.
         * 
         * @sa hasher()
         */
        std::unique_ptr<CryptoPP::SHA3> _hasher{ nullptr };
        std::vector<std::byte> encodedData{};
        /**
         * @brief Data buffer for encoder to encode.
         * 
         * The buffer of data used by the encoder to encode data.
         * 
         * @sa setBuffer(std::vector<std::byte>)
         * @sa setBuffer(std::string_view)
         * @sa setBuffer(const std::byte*, std::size_t)
         */
        std::vector<std::byte> buffer{};
    };
}