/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <algorithm>
#include <cassert>
#include <QDebug>
#include <QString>
#include <vector>

#include "TestSubject.hpp"

namespace TestSubject
{
    void unitTests()
    {
        DCTAlgoTest();
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
}