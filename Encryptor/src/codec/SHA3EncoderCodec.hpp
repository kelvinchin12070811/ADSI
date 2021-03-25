/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cryptopp/sha3.h>
#include <cstddef>
#include <memory>
#include <vector>

#include <codec/ICodec.hpp>

namespace codec {
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
     * @sa SHA3EncoderCodec(const std::byte*, std::size_t, std::unique_ptr<CryptoPP::SHA3> hasher)
     */
    explicit SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);
    /**
     * @brief Construct a new encoder with raw bytes.
     * @param data Data to encode.
     * @param hasher Hasher instance to use, nullptr for default hash instance.
     *
     * @sa SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3>)
     * @sa SHA3EncoderCodec(std::string_view, std::unique_ptr<CryptoPP::SHA3>)
     * @sa SHA3EncoderCodec(const std::byte*, std::size_t, std::unique_ptr<CryptoPP::SHA3> hasher)
     */
    explicit SHA3EncoderCodec(std::vector<std::byte> data,
                              std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);
    /**
     * @brief Construct a new encoder with encoder with bytes from std::string.
     *
     * @param data Data to encode.
     * @param hasher Hasher instance to use, nullptr for default hash instance.
     *
     * @sa SHA3EncoderCodec(std::unique_ptr<CryptoPP::SHA3>)
     * @sa SHA3EncoderCodec(std::vector<std::byte>, std::unique_ptr<CryptoPP::SHA3>)
     * @sa SHA3EncoderCodec(const std::byte*, std::size_t, std::unique_ptr<CryptoPP::SHA3> hasher)
     */
    explicit SHA3EncoderCodec(std::string_view data,
                              std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);
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
     */
    explicit SHA3EncoderCodec(const std::byte *data, std::size_t size,
                              std::unique_ptr<CryptoPP::SHA3> hasher = nullptr);

    const std::vector<std::byte> &getCodecResult() const override;
    void setCodecData(std::vector<std::byte> data) override;
    void setCodecData(const std::byte *data, std::size_t size) override;
    void setCodecData(std::string_view data) override;
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
    std::unique_ptr<CryptoPP::SHA3> _hasher { nullptr };
    /**
     * @brief Data buffer for encoder to encode.
     *
     * The buffer of data used by the encoder to encode data.
     *
     * @sa setBuffer(std::vector<std::byte>)
     * @sa setBuffer(std::string_view)
     * @sa setBuffer(const std::byte*, std::size_t)
     */
    std::vector<std::byte> _buffer {};
    /**
     * @brief Encoded data that produced by the encoder.
     */
    std::vector<std::byte> _encodedData {};
};
}