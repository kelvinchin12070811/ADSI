#pragma once
#include <algorithm>
#include <boost/math/constants/constants.hpp>
#include <numeric>

namespace utils
{
    class DCT
    {
    public:
        template <typename Container, typename Iterator>
        static void transfrom(Iterator inputBegin, Iterator inputEnd, Iterator outputBegin)
        {
            namespace boost_const = boost::math::float_constants;
            constexpr float quarter = 1.f / 4.f;
            std::transform(
                inputBegin,
                inputEnd,
                outputBegin,
                [&, u = -1](const auto& row) mutable {
                    u++;
                    Container::value_type nwRow{};
                    std::transform(
                        row.begin(),
                        row.end(),
                        nwRow.begin(),
                        [&, v = -1](const auto&) mutable {
                            v++;
                            const float aU{ (u == 0) ? boost_const::one_div_root_two : 1.f };
                            const float aV{ (v == 0) ? boost_const::one_div_root_two : 1.f };
                            float nwValue{
                                std::accumulate(
                                    inputBegin,
                                    inputEnd,
                                    0.f,
                                    [&, x = -1](const auto &rsltPrev, const auto &curRow) mutable {
                                        x++;
                                        return std::accumulate(
                                            curRow.begin(),
                                            curRow.end(),
                                            rsltPrev,
                                            [&, y = -1](const auto& prevNum, const auto& curNum) mutable {
                                                y++;
                                                return prevNum + (
                                                    (curNum - 128.f) *
                                                    std::cos((((2.f * x) + 1.f) * u * boost_const::pi) / 16.f) *
                                                    std::cos((((2.f * y) + 1.f) * v * boost_const::pi) / 16.f)
                                                );
                                            }
                                        );
                                    }
                                );
                            };
                            nwValue *= quarter * aU * aV;
                            return nwValue;
                        }
                    );
                }
                return nwRow;
            );
        }
    };
}