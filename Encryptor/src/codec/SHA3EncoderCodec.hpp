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
    private:
        /**
         * @brief SHA3 hasher instance used in hashing data.
         * 
         * The CryptoPP SHA3 hasher instance used in hashing data.
         * 
         * See also:
         * @sa hasher()
        */
        std::unique_ptr<CryptoPP::SHA3> _hasher{ nullptr };
        std::vector<std::byte> encodedData{};
        std::vector<std::byte> buffer{};
    };
}