#include <algorithm>
#include <boost/math/constants/constants.hpp>
#include <numeric>

#include "DCT.hpp"

namespace utils
{
    std::vector<std::vector<float>> DCT::transfrom(const std::vector<std::vector<float>> &input)
    {
        namespace boost_const = boost::math::float_constants;
        std::vector<std::vector<float>> rsltTransform;
        rsltTransform.reserve(input.size());

        std::transform(
            input.begin(),
            input.end(),
            rsltTransform.begin(),
            [&, u = -1](const auto &row) mutable {
                u++;
                decltype(rsltTransform)::value_type rsltNwRow;
                rsltNwRow.reserve(row.size());

                std::transform(
                    row.begin(),
                    row.end(),
                    rsltNwRow.begin(),
                    [&, v = -1](const auto &) mutable{
                        v++;
                        const float aU{ u == 0 ? boost_const::one_div_root_two : 1.f };
                        const float aV{ v == 0 ? boost_const::one_div_root_two : 1.f };
                        float nwValue{
                            std::accumulate(
                                input.begin(),
                                input.end(),
                                0.f,
                                [&, x == -1](const auto &prevSum, const auto &curRow) mutable {
                                    x++;
                                    return std::accumulate(
                                        curRow.begin(),
                                        curRow.end(),
                                        prevSum,
                                        [&, y = -1](const auto &prevSum, const auto &curNum) mutable {
                                            y++;
                                            return prevSum + (
                                                (curNum - 128.f) *
                                            );
                                        }
                                    );
                                }
                            )
                        };
                        return nwValue;
                    }
                );
            }
        );

        return rsltTransform;
    }
}