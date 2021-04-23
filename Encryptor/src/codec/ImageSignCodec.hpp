/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <QImage>

#include "codec/ICodec.hpp"
#include "db/data/Author.hpp"
#include "generator/ICryptoKeyGenerator.hpp"

namespace codec {
/**
 * @brief Codec to sign image.
 */
class ImageSignCodec : public ICodec
{
public:
    /**
     * @brief Create codec and assign required data to the codec.
     * @param image Image to sign.
     * @param pbKey Observer pointer to the public key of the author, must not be nullptr.
     * @param prKey Observer pointer to the private key of the author, must not be nullptr.
     * @param author Observer pointer to the author info of the author, must not be nullptr.
     * @throw std::invalid_argument if @p pbKey, @p prKey or @p author is nullptr.
     * 
     * @note This constructor did require temporary ownership to observe the parameter in form of observer pointer,
     * make sure this instance has been destroyed before destroying related parameter it observed to.
     */
    explicit ImageSignCodec(QImage image, const key_generator::ICryptoKeyGenerator *pbKey,
                            const key_generator::ICryptoKeyGenerator *prKey,
                            const db::data::Author *author);
    
    /**
     * @brief Not implemented as QImage can't be described in these form.
     * @throw std::logic_error if get's called.
     */
    void setCodecData(std::vector<std::byte>) override;
    /**
     * @brief Not implemented as QImage can't be described in these form.
     * @throw std::logic_error if get's called.
     */
    void setCodecData(std::string_view) override;
    /**
     * @brief Not implemented as QImage can't be described in these form.
     * @throw std::logic_error if get's called.
     */
    void setCodecData(const std::byte *, std::size_t) override;

    void execute() override;

    /**
     * @brief Not implemented as QImage can't be described in these form.
     * @throw std::logic_error if get's called.
     */
    const std::vector<std::byte> &getCodecResult() const override;
    
    /**
     * @brief Get encoded image from the codec.
     * @return Image encoded by the codec.
     * @note This member function will not keep the copy of encoded image.
     */
    virtual QImage getEncodedImage();
    /**
     * @brief Get signing receipt which as prove for author have signed the image.
     * @return Raw singing receipt text encoded with base64.
     * @note This member function will not keep the copy of encoded image.
     */
    virtual std::string getSigningReceipt();

private:
    /**
     * @brief Image to sign
     */
    QImage buffer_;
    /**
     * @brief Signed image.
     */
    QImage encoded_;
    /**
     * @brief Signing receipt of the signed image
     */
    std::string signingReceipt_;
    /**
     * @brief Observer pointer to author's public key.
     */
    const key_generator::ICryptoKeyGenerator *pbKey_ { nullptr };
    /**
     * @brief Observer pointer to author's private key.
     */
    const key_generator::ICryptoKeyGenerator *prKey_ { nullptr };
    /**
     * @brief Observer pointer to author's detailed information.
     */
    const db::data::Author *author_ { nullptr };
};
}