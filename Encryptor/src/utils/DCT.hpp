/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
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