/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha3.h>
#include <memory>
#include <QDebug>
#include <QString>
#include <vector>

#include "codec/AESEncoderCodec.hpp"
#include "codec/SHA3EncoderCodec.hpp"
#include "generator/AESCryptoKeyGenerator.hpp"
#include "TestSubject.hpp"
#include "utils/DCT.hpp"

namespace TestSubject
{
    void unitTests()
    {
        DCTAlgoTest();
        SHA3HasherTest();
        AESKeyGenTest();
        AESCodecTest();
    }
    
    void DCTAlgoTest()
    {
        constexpr std::string_view unitTestName{ "DCT transform test" };
        std::vector<std::vector<float>> data{ {
            { 52.0f, 55.0f, 61.0f, 66.0f, 70.0f, 61.0f, 64.0f, 73.0f },
            { 63.0f, 59.0f, 55.0f, 90.0f, 109.0f, 85.0f, 69.0f, 72.0f },
            { 62.0f, 59.0f, 68.0f, 113.0f, 144.0f, 104.0f, 66.0f, 73.0f },
            { 63.0f, 58.0f, 71.0f, 122.0f, 154.0f, 106.0f, 70.0f, 69.0f },
            { 67.0f, 61.0f, 68.0f, 104.0f, 126.0f, 88.0f, 68.0f, 70.0f },
            { 79.0f, 65.0f, 60.0f, 70.0f, 77.0f, 68.0f, 58.0f, 75.0f },
            { 85.0f, 71.0f, 64.0f, 59.0f, 55.0f, 61.0f, 65.0f, 83.0f },
            { 87.0f, 79.0f, 69.0f, 68.0f, 65.0f, 76.0f, 78.0f, 94.0f }
        } };

        utils::DCT dctTransform;
        std::vector<std::vector<float>> result{ dctTransform.transfrom(data) };
        auto iresult = dctTransform.itransform(result);
        auto matchResult = std::mismatch(
            data.begin(),
            data.end(),
            iresult.begin(),
            iresult.end(),
            [](const auto &lhs, const auto &rhs) {
                return std::mismatch(
                    lhs.begin(),
                    lhs.end(),
                    rhs.begin(),
                    rhs.end(),
                    [](const auto &lhs, const auto &rhs) {
                        return std::roundf(lhs) == std::roundf(rhs);
                    }
                ).second == rhs.end();
            }
        );

        if (matchResult.second != iresult.end())
        {
            qDebug() << QString{ outputTemplate.data() }.arg(unitTestName.data()).arg("Data matched: failed");
            qDebug() << QString{ "idx: %1" }.arg(std::distance(iresult.begin(), matchResult.second));
            assert(false);
        }
        else
        {
            qDebug() << QString{ outputTemplate.data() }.arg(unitTestName.data())
                .arg("Data matched: passed");
        }
    }

    void SHA3HasherTest()
    {
        const auto testName = QString{ outputTemplate.data() }.arg("SHA3 Encoder Test");

        std::string text{ "A quick brown fox jumps over the lazy dog" };
        std::string encCryptoPP;
        std::string encCodec;
        CryptoPP::SHA3_256 encSHA3_256;
        static_cast<void>(CryptoPP::StringSource{
            text,
            true,
            new CryptoPP::HashFilter{
                encSHA3_256,
                new CryptoPP::HexEncoder{
                    new CryptoPP::StringSink{ encCryptoPP }
                }
            }
        });

        codec::SHA3EncoderCodec encSHA3_256_2{ text };
        encSHA3_256_2.execute();
        auto data = encSHA3_256_2.getCodecResult();
        static_cast<void>(CryptoPP::ArraySource{
            reinterpret_cast<CryptoPP::byte*>(data.data()),
            data.size(),
            true,
            new CryptoPP::HexEncoder{
                new CryptoPP::StringSink{ encCodec }
            }
        });

        bool matched = encCryptoPP == encCodec;
        qDebug() << testName.arg(
            QStringLiteral("SHA3 product match: %1")
                .arg(matched ? QStringLiteral("passed") : QStringLiteral("failed"))
        );
        assert(matched);
    }
    
    void AESKeyGenTest()
    {
        constexpr std::string_view password{ "test" };
        constexpr std::string_view preCalHash{ "F220CF02E8E0BF291A08D1BFE53E989DAE21F2BF20D919CF8A80BBC866C98C62" };
        const auto testName = QString{ outputTemplate.data() }.arg("AES Key Test");

        key_generator::AESCryptoKeyGenerator generator{ password.data() };
        generator.generate();
        auto key = generator.getGeneratedKey();

        CryptoPP::SHA3_256 sha3_256Digester;
        std::string pwHash;
        static_cast<void>(CryptoPP::ArraySource{
            reinterpret_cast<CryptoPP::byte*>(key.data()),
            key.size(),
            true,
            new CryptoPP::HashFilter{
                sha3_256Digester,
                new CryptoPP::HexEncoder{
                    new CryptoPP::StringSink{ pwHash }
                }
            }
        });

        bool passed = pwHash == preCalHash;
        qDebug() << testName.arg(QStringLiteral("Regen random key matched: %1").arg(passed ?
            QStringLiteral("passed") : QStringLiteral("failed")));
        assert(passed);
    }
    
    void AESCodecTest()
    {
        const auto testName = QString{ outputTemplate.data() }.arg("AES Codec Test");
        constexpr std::string_view data{ "A quick brown fox jumps over the lazy dog." };
        constexpr std::string_view password{ "ADamnSuperStrongPassword" };
        std::vector<std::byte> AESKey;

        key_generator::AESCryptoKeyGenerator genAESKey{ password.data() };
        genAESKey.generate();
        AESKey = genAESKey.getGeneratedKey();

        codec::AESEncoderCodec encoder{ reinterpret_cast<const std::byte*>(data.data()), data.size(), AESKey };
        encoder.execute();
        auto encodedAES = encoder.getCodecResult();

        codec::SHA3EncoderCodec codSHA3_256{ encodedAES };
        codSHA3_256.execute();

        std::string aesEncodedDataHex;
        auto aesEncodedData = codSHA3_256.getCodecResult();
        static_cast<void>(CryptoPP::ArraySource{
            reinterpret_cast<CryptoPP::byte*>(aesEncodedData.data()),
            aesEncodedData.size(),
            true,
            new CryptoPP::HexEncoder{
                new CryptoPP::StringSink{ aesEncodedDataHex }
            }
        });

        qDebug() << testName.arg(QStringLiteral("AES Encoded hash: %1").arg(QString::fromStdString(aesEncodedDataHex)));
    }
}