#pragma once
#include <vector>

namespace utils
{
    class DCT
    {
    public:
        static std::vector<std::vector<float>> transfrom(const std::vector<std::vector<float>> &input);
    };
}