/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstddef>
#include <memory>
#include <string_view>
#include <variant>
#include <vector>

#include "codec/ICodec.hpp"
#include "codec/ImageSignCodec.hpp"
#include "db/data/Author.hpp"
#include "generator/ICryptoKeyGenerator.hpp"

namespace codec {
/**
 * @brief Interface of factory that control the creation of the codecs available.
 */
struct ICodecFactory
{
    /**
     * @brief Represented as the array type of data.
     */
    using ArrayDataType = std::vector<std::byte>;
    /**
     * @brief Represented as string type of data.
     */
    using StringDataType = std::string_view;
    /**
     * @brief Represented as C style array type.
     */
    using CStyleArrayDataType = std::pair<const std::byte *, std::size_t>;
    /**
     * @brief Type of data supported by the codec.
     */
    using CodecDataStream = std::variant<ArrayDataType, StringDataType, CStyleArrayDataType>;

    /**
     * @brief Create default binary to text encoder.
     * @param data Data to pass into the encoder.
     * @return New encoder consturcted.
     */
    virtual std::unique_ptr<ICodec> createDefaultB2TEncoder(CodecDataStream data = {}) = 0 { }
    /**
     * @brief Create default text to bianry coder.
     * @param data Data to pass into the coder.
     * @return Default decoder used to decode text data into binary.
     */
    virtual std::unique_ptr<ICodec> createDefaultB2TDecoder(CodecDataStream data = {}) = 0 { }
    /**
     * @brief Create default hash encoder.
     * @param data Data to pass into the encoder.
     * @return New hash encoder with provided data.
     */
    virtual std::unique_ptr<ICodec> createDefaultHashEncoder(CodecDataStream data = {}) = 0 { }
    /**
     * @brief Create default symmetric encryption encoder.
     * @param data Data to encrypt.
     * @param key Key generator to generate default key.
     * @return Default symmetric encoder to encrypt data.
     * @throw std::invalid_argument if key is not applicable to target codec.
     * @note If @p key did not generate any key yet then it will generate one.
     */
    virtual std::unique_ptr<ICodec>
    createDefaultSymCryptoEncoder(CodecDataStream data,
                                   key_generator::ICryptoKeyGenerator *key) = 0
    {
    }
    /**
     * @brief Create default symmetric encyption decoder.
     * @param data Data to decrypt.
     * @param key Key generator to generate default key.
     * @return Default symmetric decoder to decrypt data.
     * @throw std::invalid_argument if @p key is not applicable to target codec.
     * @note If @p key did not generate any key yet then it will generate one.
     */
    virtual std::unique_ptr<ICodec>
    createDefaultSymCryptoDecoder(CodecDataStream data, key_generator::ICryptoKeyGenerator *key) = 0
    {
    }
    /**
     * @brief Create default asymmetric enctyption encoder.
     * @param data Data to encrypt.
     * @param key Key generator to generate default key.
     * @return Default asymmetric encoder to encrypt data.
     * @throw std::invalid_argument if @p key is not applicable to target codec.
     * @note If @p key did not generate any key yet then it will generate one.
     */
    virtual std::unique_ptr<ICodec>
    createDefaultASymCryptoEncryptor(CodecDataStream data,
                                     key_generator::ICryptoKeyGenerator *key) = 0
    {
    }
    /**
     * @brief Create default compression codec.
     * @param data Data to compress.
     * @return Default compression codec.
     */
    virtual std::unique_ptr<ICodec> createDefaultCompresssCoder(CodecDataStream data = {}) = 0 { }
    /**
     * @brief Create default decompression codec.
     * @param data Data to uncompress.
     * @return Default decompression codec.
     */
    virtual std::unique_ptr<ICodec> createDefaultDecompressCoder(CodecDataStream data = {}) = 0 { }
    /**
     * @brief Create codec and assign required data to the codec.
     * @param image Image to sign.
     * @param pbKey Observer pointer to the public key of the author, must not be nullptr.
     * @param prKey Observer pointer to the private key of the author, must not be nullptr.
     * @param author Observer pointer to the author info of the author, must not be nullptr.
     * @return Instance of ImageSignCodec to sign image.
     * @throw std::invalid_argument if @p pbKey, @p prKey or @p author is nullptr.
     *
     * @note This member function did require temporary ownership to observe the parameter in form of
     * observer pointer, make sure the instance has been destroyed before destroying related
     * parameter it observed to.
     */
    virtual std::unique_ptr<ImageSignCodec>
    createDefaultImageSigner(QImage image, const key_generator::ICryptoKeyGenerator *pbKey,
                             const key_generator::ICryptoKeyGenerator *prKey,
                             const db::data::Author *author) = 0
    {
    }
};
}