/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/#include <algorithm>
#include <boost/math/constants/constants.hpp>
#include <iterator>
#include <numeric>
#include <QDebug>

#include "DCT.hpp"

namespace utils
{
    std::vector<std::vector<float>> DCT::transfrom(const std::vector<std::vector<float>> &input)
    {
        std::vector<std::vector<float>> rsltTransform;
        rsltTransform.reserve(input.size());
        transromVector(input, rsltTransform);
        return rsltTransform;
    }
    
    void DCT::transromVector(const std::vector<std::vector<float>> &input, std::vector<std::vector<float>> &rslt)
    {
        namespace boost_const = boost::math::float_constants;
        auto u = static_cast<std::size_t>(-1);
        auto v = static_cast<std::size_t>(-1);
        constexpr float quarter = 1.f / 4.f;

        std::transform(
            input.begin(),
            input.end(),
            std::back_inserter(rslt),
            [&](const auto &row) {
                u >= input.size() - 1 ? u = 0 : u++;
                std::vector<float> rsltRow;
                rsltRow.reserve(8);
                std::transform(
                    row.begin(),
                    row.end(),
                    std::back_inserter(rsltRow),
                    [&](const auto &) {
                        v >= row.size() - 1 ? v = 0 : v++;
                        const float aU{ u == 0 ? boost_const::one_div_root_two : 1.f };
                        const float aV{ v == 0 ? boost_const::one_div_root_two : 1.f };
                        float rsltSum{ dctAdder(input, u, v) };
                        rsltSum *= quarter * aU * aV;
                        return rsltSum;
                    }
                );

                return rsltRow;
            }
        );
    }
    
    float DCT::dctAdder(const std::vector<std::vector<float>> &input, int u, int v)
    {
        namespace boost_const = boost::math::float_constants;
        auto x = static_cast<std::size_t>(-1);
        auto y = static_cast<std::size_t>(-1);
        
        return std::accumulate(
            input.begin(),
            input.end(),
            0.f,
            [&](const auto &prevRowSum, const auto &curRow) {
                x >= input.size() - 1 ? x = 0 : x++;
                return std::accumulate(
                    curRow.begin(),
                    curRow.end(),
                    prevRowSum,
                    [&](const auto &prevSum, const auto &curVal) {
                        y >= curRow.size() - 1 ? y = 0 : y++;

                        return prevSum + (
                            (curVal - 128.f) *
                            std::cos((((2.f * static_cast<float>(x)) + 1.f) *
                            static_cast<float>(u) * boost_const::pi) / 16.f) *
                            std::cos((((2.f * static_cast<float>(y)) + 1.f) *
                            static_cast<float>(v) * boost_const::pi) / 16.f)
                        );
                    }
                );
            }
        );
    }
}