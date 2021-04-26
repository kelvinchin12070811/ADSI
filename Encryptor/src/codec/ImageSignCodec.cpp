/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QColor>
#include <QDebug>

#include <array>
#include <bitset>
#include <cmath>
#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/range/irange.hpp>

#include "codec/ImageSignCodec.hpp"
#include "codec/DefaultCodecFactory.hpp"
#include "generator/PublicRSACryptoKeyGenerator.hpp"
#include "utils/DCT.hpp"

#ifdef DEBUG
#include <QFile>
#include <fstream>
#include <iterator>
#endif // DEBUG


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

    signingReceipt_ = buildSignatureText();
    /*auto itrSignature = signingReceipt_.begin();
    if (signingReceipt_.size() >= (static_cast<std::size_t>(col) * row) * 4 * 8)
        throw std::length_error { "Image not large enough to hold the signature." };

    for (auto grpY : boost::irange(row)) {
        for (auto grpX : boost::irange(col)) {
            if (itrSignature == signingReceipt_.end()) return;

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
            auto prevBlock = block;
            std::bitset<8> bufSignatureBits {};
            unsigned short bitsRemain { 0 };
            std::array<std::array<int, 2>, 4> posMidFreqCoefficients {
                { { 1, 4 }, { 2, 3 }, { 3, 2 }, { 4, 1 } }
            };
            for (auto idx : boost::irange(4)) {
                if (bitsRemain == 0) {
                    if (itrSignature == signingReceipt_.end()) break;

                    auto next = static_cast<std::uint8_t>(*itrSignature);
                    bufSignatureBits = { *reinterpret_cast<std::uint8_t *>(&next) };
                    bitsRemain = 8;
                    itrSignature++;
                }

                auto &&[posX, posY] = posMidFreqCoefficients[idx];
                auto &selectedCoefficient = dctedBlock[posY][posX];
                selectedCoefficient = bufSignatureBits.test(8 - bitsRemain) ? 255.f : 0.f;
                bitsRemain--;
            }

            block = dct.itransform(dctedBlock);
            assert(block != prevBlock);

            for (auto itrJ = block.begin(); itrJ != block.end(); itrJ++) {
                for (auto itrI = itrJ->begin(); itrI != itrJ->end(); itrI++) {
                    QPoint pos { static_cast<int>(std::distance(itrJ->begin(), itrI) * grpX),
                                 static_cast<int>(std::distance(block.begin(), itrJ) * grpY) };
                    auto pixColor = encoded_.pixelColor(pos);

                    if (pixColor.isValid()) pixColor.setBlueF(std::roundf(*itrI));
                    encoded_.setPixelColor(pos, pixColor);
                }
            }

            auto rawProgress = static_cast<float>(std::distance(signingReceipt_.begin(), itrSignature))
                    / static_cast<float>(signingReceipt_.size());
            auto percentProgress = rawProgress * 100.f;
            emit progressUpdated(percentProgress);
        }
    }*/
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
    dataBuffer.reserve(sizeof(szData) + dmpPbKey.length());
    for (auto idx : boost::irange(sizeof(std::uint16_t)))
        dataBuffer.push_back(reinterpret_cast<const std::byte *>(&szData)[idx]);
    std::transform(dmpPbKey.begin(), dmpPbKey.end(), std::back_inserter(dataBuffer),
                   [](const auto &elm) { return static_cast<std::byte>(elm); });

    BOOST_ASSERT(dataBuffer.size() == dataBuffer.capacity());

    szData = static_cast<decltype(szData)>(author_->authorName.length());
    BOOST_ASSERT(szData < std::numeric_limits<decltype(szData)>::max());
    dataBuffer.reserve(dataBuffer.size() + sizeof(szData) + szData);
    for (auto idx : boost::irange(sizeof(szData)))
        dataBuffer.emplace_back(reinterpret_cast<const std::byte *>(&szData)[idx]);
    std::transform(author_->authorName.begin(), author_->authorName.end(),
                   std::back_inserter(dataBuffer),
                   [](const auto &elm) { return static_cast<std::byte>(elm); });

    BOOST_ASSERT(dataBuffer.size() == dataBuffer.capacity());

    szData = static_cast<decltype(szData)>(author_->authorEmail.length());
    BOOST_ASSERT(szData < std::numeric_limits<decltype(szData)>::max());
    dataBuffer.reserve(dataBuffer.size() + sizeof(szData) + szData);
    for (auto idx : boost::irange(sizeof(szData)))
        dataBuffer.emplace_back(reinterpret_cast<const std::byte *>(&szData)[idx]);
    std::transform(author_->authorEmail.begin(), author_->authorEmail.end(),
                   std::back_inserter(dataBuffer),
                   [](const auto &elm) { return static_cast<std::byte>(elm); });

    BOOST_ASSERT(dataBuffer.size() == dataBuffer.capacity());

    szData = static_cast<decltype(szData)>(author_->authorPortFolioURL.length());
    BOOST_ASSERT(szData < std::numeric_limits<decltype(szData)>::max());
    dataBuffer.reserve(dataBuffer.size() + sizeof(szData) + szData);
    for (auto idx : boost::irange(sizeof(szData)))
        dataBuffer.emplace_back(reinterpret_cast<const std::byte *>(&szData)[idx]);
    std::transform(author_->authorPortFolioURL.begin(), author_->authorPortFolioURL.end(),
                   std::back_inserter(dataBuffer),
                   [](const auto &elm) { return static_cast<std::byte>(elm); });

    BOOST_ASSERT(dataBuffer.size() == dataBuffer.capacity());

    auto signer = facCodec->createDefaultASymCryptoEncryptor(
            dataBuffer, const_cast<key_generator::ICryptoKeyGenerator *>(prKey_));
    signer->execute();
    auto &&result = signer->getCodecResult();
    BOOST_ASSERT(result.size() <= std::numeric_limits<std::uint16_t>::max());
    szData = static_cast<std::uint16_t>(result.size());
    dataBuffer.reserve(dataBuffer.size() + sizeof(std::uint16_t) + result.size());
    for (auto idx : boost::irange(sizeof(szData)))
        dataBuffer.emplace_back(reinterpret_cast<const std::byte *>(&szData)[idx]);
    std::transform(result.begin(), result.end(), std::back_inserter(dataBuffer),
                   [](const auto &elm) { return static_cast<std::byte>(elm); });

    BOOST_ASSERT(dataBuffer.size() == dataBuffer.capacity());

#ifdef DEBUG
    std::ofstream debugOut;
    debugOut.open("_debug.dat", std::ios::binary | std::ios::out);
    if (debugOut.is_open()) {
        std::transform(dataBuffer.begin(), dataBuffer.end(),
                       std::ostream_iterator<std::uint8_t>(debugOut),
                       [](const auto &elm) { return static_cast<std::uint8_t>(elm); });
        debugOut.close();
    }
#endif // DEBUG

    return dataBuffer;
    auto compressCodec = facCodec->createDefaultCompresssCoder(std::move(dataBuffer));
    compressCodec->execute();
    //return compressCodec->getCodecResult();
}

QImage ImageSignCodec::getEncodedImage()
{
    return encoded_;
}

std::string ImageSignCodec::getSigningReceipt()
{
    std::unique_ptr<codec::ICodecFactory> facCodec {
        std::make_unique<codec::DefaultCodecFactory>()
    };
    auto compressCoder = facCodec->createDefaultCompresssCoder(signingReceipt_);
    compressCoder->execute();
    auto b2tEncoder = facCodec->createDefaultB2TEncoder(compressCoder->getCodecResult());
    b2tEncoder->execute();
    const auto &result = b2tEncoder->getCodecResult();
    auto begResult = reinterpret_cast<const char *>(result.data());
    std::string receipt { begResult, begResult + result.size() };
    return receipt;
}
}