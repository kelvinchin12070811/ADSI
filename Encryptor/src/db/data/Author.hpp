/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstdint>
#include <string>

namespace db::data {
struct Author
{
    Author(std::uint32_t authorID, std::string authorName, std::string authorEmail,
           std::string authorPortFolioURL)
        : authorID { authorID },
          authorName { authorName },
          authorEmail { authorEmail },
          authorPortFolioURL { authorPortFolioURL }
    {
    }
    
    Author(std::string authorName, std::string authorEmail, std::string authorPortFolioURL)
        : Author(-1u, authorName, authorEmail, authorPortFolioURL)
    {
    }

    std::uint32_t authorID;
    std::string authorName;
    std::string authorEmail;
    std::string authorPortFolioURL;
};
}