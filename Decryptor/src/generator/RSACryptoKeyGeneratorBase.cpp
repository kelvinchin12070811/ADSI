/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <cryptopp/osrng.h>

#include "generator/RSACryptoKeyGeneratorBase.hpp"

namespace key_generator {
CryptoPP::InvertibleRSAFunction RSACryptoKeyGeneratorBase::generateKeyParams()
{
    CryptoPP::AutoSeededRandomPool rdnPool;
    CryptoPP::InvertibleRSAFunction keyParams;
    keyParams.GenerateRandomWithKeySize(rdnPool, 3072);
    return keyParams;
}
}