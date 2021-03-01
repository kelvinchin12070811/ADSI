#define BOOST_TEST_MODULE ADSI Encryptor Unit Test
#include <boost/test/unit_test.hpp>

#include <algorithm>

#include "utils/DCT.hpp"

BOOST_AUTO_TEST_CASE(dct_algo_test)
{
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

    utils::DCT transform;
    auto result = transform.transfrom(data);
    auto iresult = transform.itransform(result);

    auto [firstResult, secondResult] = std::mismatch(
        iresult.begin(),
        iresult.end(),
        data.begin(),
        [](const auto &lhs, const auto &rhs) {
            auto [first, second] = std::mismatch(
                lhs.begin(),
                lhs.end(),
                rhs.begin(),
                [](const auto &lhs, const auto &rhs) { return std::roundf(lhs) == std::roundf(rhs); }
            );
            return (first == lhs.end()) && (second == rhs.end());
        }
    );
    BOOST_REQUIRE((firstResult == iresult.end()) && (secondResult == data.end()));
}