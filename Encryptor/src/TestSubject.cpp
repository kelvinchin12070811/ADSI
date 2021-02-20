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
        constexpr std::string_view preCalHash{ "3BFDF0A136BFA6E5C4883DCAD52213E3888F7098150664A06B244DC601A6F48A" };
        const auto testName = QString{ outputTemplate.data() }.arg("AES Key Test");

        key_generator::AESCryptoKeyGenerator generator{ password.data() };
        generator.generate();
        auto key = generator.getGeneratedKey();

        CryptoPP::SHA3_256 sha3_256Digester;
        std::string pwHash;
        std::make_unique<CryptoPP::ArraySource>(
            reinterpret_cast<CryptoPP::byte*>(key.data()),
            key.size(),
            true,
            new CryptoPP::HashFilter{
                sha3_256Digester,
                new CryptoPP::HexEncoder{
                    new CryptoPP::StringSink{ pwHash }
                }
            }
        );

        bool passed = pwHash == preCalHash;
        qDebug() << testName.arg(QStringLiteral("Regen random key matched: %1").arg(passed ?
            QStringLiteral("passed") : QStringLiteral("failed")));
        assert(passed);
    }
}