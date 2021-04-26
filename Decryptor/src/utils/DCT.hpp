/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <vector>

namespace utils {
/**
 * @brief Utility class that provide DCT transform algorithm.
 *
 * This utility transfrom std::vector into DCT wave or DCT wave to raw data.
 */
class DCT
{
public:
    /**
     * Transfrom std::vector into 2D DCT wave.
     * @param input Input data.
     * @return 2D DCT wave.
     */
    std::vector<std::vector<float>> transfrom(const std::vector<std::vector<float>> &input);
    /**
     * Transfrom std::vector of 2D DCT wave to raw data.
     * @param input DCT wave.
     * @return Inverse transform of 2D DCT.
     */
    std::vector<std::vector<float>> itransform(const std::vector<std::vector<float>> &input);

private:
    /**
     * Helper function of converting raw data into DCT wave.
     * @param input Data input.
     * @param rslt DCT wave output
     */
    void transfromVector(const std::vector<std::vector<float>> &input,
                         std::vector<std::vector<float>> &rslt);
    /**
     * Helper function of converting DCT wave into raw data.
     * @param input DCT data input.
     * @param rslt Raw data output.
     */
    void itransfromVector(const std::vector<std::vector<float>> &input,
                          std::vector<std::vector<float>> &rslt);
    /**
     * Helper function of adding data of raw data with provided u and v variable.
     * @param input Data input
     * @param u u variable in DCT
     * @param v v variable in DCT
     * @return DCT value of data at [v][u].
     */
    float dctAdder(const std::vector<std::vector<float>> &input, int u, int v);
    /**
     * Helper function of adding data of DCT data with provided x and y variable.
     * @param input DCT data input.
     * @param x x varaible in DCT.
     * @param y y varaible in DCT.
     * @return Inverse DCT value of data at [y][x].
     */
    float idctAdder(const std::vector<std::vector<float>> &input, int x, int y);
};
}