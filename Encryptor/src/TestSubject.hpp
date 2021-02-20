/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <string_view>

namespace TestSubject
{
    constexpr std::string_view outputTemplate{ "[%1]: %2" };

    void unitTests();
    void DCTAlgoTest();
    void SHA3HasherTest();
    void AESKeyGenTest();
}