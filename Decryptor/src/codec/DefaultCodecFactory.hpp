/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include "codec/ICodecFactory.hpp"

namespace codec {
/**
 * @brief Default factory implementation to create codec::ICodec objects.
 */
class DefaultCodecFactory : public ICodecFactory
{
public:
    std::unique_ptr<ICodec> createDefaultB2TEncoder(CodecDataStream data) override;
    std::unique_ptr<ICodec> createDefaultB2TDecoder(CodecDataStream data) override;
    std::unique_ptr<ICodec> createDefaultHashEncoder(CodecDataStream data) override;
    std::unique_ptr<ICodec>
    createDefaultSymCryptoEncoder(CodecDataStream data,
                                  key_generator::ICryptoKeyGenerator *key) override;
    std::unique_ptr<ICodec>
    createDefaultSymCryptoDecoder(CodecDataStream data,
                                  key_generator::ICryptoKeyGenerator *key) override;
    std::unique_ptr<ICodec>
    createDefaultASymCryptoEncryptor(CodecDataStream data,
                                     key_generator::ICryptoKeyGenerator *key) override;
    std::unique_ptr<ICodec> createDefaultCompresssCoder(CodecDataStream data) override;
    std::unique_ptr<ICodec> createDefaultDecompressCoder(CodecDataStream data) override;
    std::unique_ptr<ImageSignCodec>
    createDefaultImageSigner(QImage image, const key_generator::ICryptoKeyGenerator *pbKey,
                             const key_generator::ICryptoKeyGenerator *prKey,
                             const db::data::Author *author);

private:
    /**
     * @brief Apply data to codec.
     * @param data Data to apply.
     * @param codec Codec to apply data.
     */
    void setCodecBuffer(CodecDataStream data, ICodec *codec);
};
}