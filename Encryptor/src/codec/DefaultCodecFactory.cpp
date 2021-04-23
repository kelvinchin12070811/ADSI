/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <stdexcept>

#include "codec/DefaultCodecFactory.hpp"
#include "codec/AESDecoderCodec.hpp"
#include "codec/AESEncoderCodec.hpp"
#include "codec/Base64DecoderCodec.hpp"
#include "codec/Base64EncoderCodec.hpp"
#include "codec/DeflateCodec.hpp"
#include "codec/InflateCodec.hpp"
#include "codec/RSASignEncoderCodec.hpp"
#include "codec/SHA3EncoderCodec.hpp"
#include "generator/AESCryptoKeyGenerator.hpp"
#include "generator/PrivateRSACryptoKeyGenerator.hpp"
#include "generator/PublicRSACryptoKeyGenerator.hpp"

namespace codec {
std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultB2TEncoder(CodecDataStream data)
{
    std::unique_ptr<ICodec> codec = std::make_unique<Base64EncoderCodec>();
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultB2TDecoder(CodecDataStream data)
{
    std::unique_ptr<ICodec> codec = std::make_unique<Base64DecoderCodec>();
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultHashEncoder(CodecDataStream data)
{
    std::unique_ptr<ICodec> codec = std::make_unique<SHA3EncoderCodec>();
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

std::unique_ptr<ICodec>
DefaultCodecFactory::createDefaultSymCryptoEncoder(CodecDataStream data,
                                                   key_generator::ICryptoKeyGenerator *key)
{
    if (dynamic_cast<key_generator::AESCryptoKeyGenerator *>(key) == nullptr) {
        throw std::invalid_argument { "Key parameter is not applicable to AES Encryptor." };
    }

    if (key->getGeneratedKey().empty()) key->generate();
    auto codec = std::make_unique<AESEncoderCodec>("", key->getGeneratedKey());
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

std::unique_ptr<ICodec>
DefaultCodecFactory::createDefaultSymCryptoDecoder(CodecDataStream data,
                                                   key_generator::ICryptoKeyGenerator *key)
{
    if (dynamic_cast<key_generator::AESCryptoKeyGenerator *>(key) == nullptr) {
        throw std::invalid_argument { "Key parameter is not applicable to AES Decryptor." };
    }

    if (key->getGeneratedKey().empty()) key->generate();
    auto codec = std::make_unique<AESDecoderCodec>("", key->getGeneratedKey());
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

std::unique_ptr<ICodec>
DefaultCodecFactory::createDefaultASymCryptoEncryptor(CodecDataStream data,
                                                      key_generator::ICryptoKeyGenerator *key)
{
    auto keyGenerator = dynamic_cast<key_generator::PrivateRSACryptoKeyGenerator *>(key);
    if (keyGenerator == nullptr) {
        throw std::invalid_argument { "Key parameter is not applicable to RSA Sign Encoder." };
    }

    if (key->getGeneratedKey().empty()) key->generate();
    auto codec = std::make_unique<RSASignEncoderCodec>("", keyGenerator->getPrivatekey());
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultCompresssCoder(CodecDataStream data)
{
    auto codec = std::make_unique<DeflatCodec>();
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

std::unique_ptr<ICodec> DefaultCodecFactory::createDefaultDecompressCoder(CodecDataStream data)
{
    auto codec = std::make_unique<InflateCodec>();
    setCodecBuffer(std::move(data), codec.get());
    return codec;
}

void DefaultCodecFactory::setCodecBuffer(CodecDataStream data, ICodec *codec)
{
    if (std::holds_alternative<ArrayDataType>(data)) {
        auto &&tmp = std::get<ArrayDataType>(data);
        codec->setCodecData(std::move(tmp));
        return;
    }

    if (std::holds_alternative<StringDataType>(data)) {
        auto &&tmp = std::get<StringDataType>(data);
        codec->setCodecData(tmp);
        return;
    }

    if (std::holds_alternative<CStyleArrayDataType>(data)) {
        const auto &[pointer, size] = std::get<CStyleArrayDataType>(data);
        codec->setCodecData(pointer, size);
        return;
    }
}
}