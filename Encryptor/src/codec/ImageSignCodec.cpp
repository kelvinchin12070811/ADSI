/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QColor>
#include <QDebug>

#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/range/irange.hpp>

#include "codec/ImageSignCodec.hpp"
#include "codec/DefaultCodecFactory.hpp"
#include "generator/PublicRSACryptoKeyGenerator.hpp"
#include "utils/DCT.hpp"

namespace codec {
ImageSignCodec::ImageSignCodec(QImage image, const key_generator::ICryptoKeyGenerator *pbKey,
                               const key_generator::ICryptoKeyGenerator *prKey,
                               const db::data::Author *author)
    : buffer_ { std::move(image) }, pbKey_ { pbKey }, prKey_ { prKey }, author_ { author }
{
    if (pbKey_ == nullptr)
        throw std::invalid_argument { "Parameter pbKey must not be nullptr but it seems to be." };

    if (prKey_ == nullptr)
        throw std::invalid_argument { "Parameter prKey must not be nulltptr but it seems to be." };

    if (author_ == nullptr)
        throw std::invalid_argument { "Parameter author must not be nullptr but it seems to be." };
}

void ImageSignCodec::setCodecData(std::vector<std::byte>)
{
    throw std::logic_error { "codec::ImageSignCodec does not support this operation" };
}

void ImageSignCodec::setCodecData(std::string_view)
{
    throw std::logic_error { "codec::ImageSignCodec does not support this operation" };
}

void ImageSignCodec::setCodecData(const std::byte *, std::size_t)
{
    throw std::logic_error { "codec::ImageSignCodec does not support this operation" };
}

void ImageSignCodec::execute()
{
    encoded_ = buffer_;
    int col = encoded_.width() / 8 + (encoded_.width() % 8 == 0 ? 0 : 1);
    int row = encoded_.height() / 8 + (encoded_.height() % 8 == 0 ? 0 : 1);

    auto signature = buildSignatureText();
    QString output;
    for (const auto &itr : signature)
        output = QStringLiteral("%1 %2").arg(output, static_cast<int>(itr));
    qDebug() << output;

    for (auto grpY : boost::irange(row)) {
        for (auto grpX : boost::irange(col)) {
            std::vector<std::vector<float>> block;
            block.resize(8);
            for (auto &&itr : block) itr.resize(8);

            for (auto itrJ = block.begin(); itrJ != block.end(); itrJ++) {
                for (auto itrI = itrJ->begin(); itrI != itrJ->end(); itrI++) {
                    QPoint pos { static_cast<int>(std::distance(itrJ->begin(), itrI) * grpX),
                                 static_cast<int>(std::distance(block.begin(), itrJ) * grpY) };
                    auto pixColor = encoded_.pixelColor(pos);

                    if (pixColor.isValid()) {
                        *itrI = pixColor.blueF();
                        continue;
                    }

                    *itrI = 0;
                }
            }

            utils::DCT dct;
            auto dctedBlock = dct.transfrom(block);
        }
    }
}

const std::vector<std::byte> &ImageSignCodec::getCodecResult() const
{
    throw std::logic_error { "codec::ImageSignCodec does not support this operation" };
}

std::vector<std::byte> ImageSignCodec::buildSignatureText()
{
    std::vector<std::byte> dataBuffer;
    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };

    std::string dmpPbKey;
    auto pbKey = dynamic_cast<const key_generator::PublicRSACryptoKeyGenerator *>(pbKey_);
    pbKey->getPublicKey().DEREncode(CryptoPP::StringSink { dmpPbKey });
    BOOST_ASSERT(dmpPbKey.length() < std::numeric_limits<std::uint16_t>::max());

    auto szData = static_cast<std::uint16_t>(dmpPbKey.length());
    dataBuffer.resize(sizeof(std::uint16_t) + dmpPbKey.length());
    for (auto idx : boost::irange(sizeof(std::uint16_t)))
        dataBuffer.push_back(reinterpret_cast<const std::byte *>(&szData)[idx]);
    std::transform(dmpPbKey.begin(), dmpPbKey.end(), std::back_inserter(dataBuffer),
                   [](const auto &elm) { return static_cast<std::byte>(elm); });

    return dataBuffer;
}

QImage ImageSignCodec::getEncodedImage()
{
    return std::move(encoded_);
}

std::string ImageSignCodec::getSigningReceipt()
{
    return std::move(signingReceipt_);
}
}