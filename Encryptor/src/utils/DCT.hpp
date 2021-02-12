#pragma once
#include <vector>

namespace utils
{
    class DCT
    {
    public:
        std::vector<std::vector<float>> transfrom(const std::vector<std::vector<float>> &input);

    private:
        void transromVector(const std::vector<std::vector<float>> &input, std::vector<std::vector<float>> &rslt);
        float dctAdder(const std::vector<std::vector<float>> &input, int u, int v);
    };
}