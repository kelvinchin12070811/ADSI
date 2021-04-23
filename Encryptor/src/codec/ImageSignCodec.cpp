/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QColor>
#include <QDebug>

#include <stdexcept>

#include <boost/range/irange.hpp>

#include "codec/ImageSignCodec.hpp"
#include "codec/ICodecFactory.hpp"
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

    for (auto grpY : boost::irange(col)) {
        for (auto grpX : boost::irange(row)) {
            std::vector<std::vector<float>> block;
            block.resize(8);
            for (auto &&itr : block) itr.resize(8);

            for (auto itrJ = block.begin(); itrJ != block.end(); itrJ++) {
                for (auto itrI = itrJ->begin(); itrI != itrJ->end(); itrI++) {
                    *itrI = encoded_.pixelColor(std::distance(itrJ->begin(), itrI),
                                                std::distance(block.begin(), itrJ))
                                    .blue();
                }
            }

            utils::DCT dct;
            auto dctedBlock = dct.transfrom(block);

            qDebug() << dctedBlock;
        }
    }
}

const std::vector<std::byte> &ImageSignCodec::getCodecResult() const
{
    throw std::logic_error { "codec::ImageSignCodec does not support this operation" };
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