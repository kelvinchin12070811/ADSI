/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <aes.h>
#include <algorithm>
#include <cassert>
#include <hex.h>
#include <memory>
#include <osrng.h>
#include <QDebug>
#include <QString>
#include <sha3.h>
#include <vector>

#include "generator/AESCryptoKeyGenerator.hpp"
#include "TestSubject.hpp"
#include "utils/DCT.hpp"

namespace TestSubject
{
    void unitTests()
    {
        DCTAlgoTest();
        AESKeyGenTest();
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
    
    void AESKeyGenTest()
    {
        constexpr std::string_view password{ "test" };
        const auto testName = QString{ outputTemplate.data() }.arg("AES Key Test");

        std::size_t szKey = CryptoPP::AES::MAX_KEYLENGTH;
        qDebug() << testName.arg(QStringLiteral("Size of AES Key: %1")).arg(szKey);
        qDebug() << testName.arg(QStringLiteral("Size of AES Key (bit): %1")).arg(szKey * 8);

        CryptoPP::SHA3_256 shaHasher;
        std::vector<CryptoPP::byte> pwHash;
        std::make_unique<CryptoPP::StringSource>(
            password,
            true,
            new CryptoPP::HashFilter(
                shaHasher,
                new CryptoPP::VectorSink(pwHash)
            )
        );

        std::string hexHash;
        std::make_unique<CryptoPP::VectorSource>(
            pwHash,
            true,
            new CryptoPP::HexEncoder
        );

        qDebug() << testName.arg(QString{ "Test password: %1" }.arg(password.data()));

        key_generator::AESCryptoKeyGenerator generator{ password.data() };
        generator.generate();
    }
}