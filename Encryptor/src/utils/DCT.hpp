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
        Container transfrom(Iterator inputBegin, Iterator inputEnd, Iterator outputBegin)
        {
            namespace boost_const = boost::math::float_constants;
            std::transform(
                inputBegin,
                inputEnd,
                outputBegin,
                [&, u = static_cast<std::size_t>(-1)](const auto& row) mutable {
                    u++;
                    Container::value_type nwRow{};
                    std::transform(
                        row.begin(),
                        row.end(),
                        nwRow.begin(),
                        [&, v = std::static_cast<std::size_t>(-1)](const auto&) mutable {
                            v++;
                            const float aU{ (u == 0) ? boost_const::one_div_root_two : 1.f };
                            const float aV{ (v == 0) ? boost_const::one_div_root_two : 1.f };
                        }
                    );
                }
            );
        }
    };
}