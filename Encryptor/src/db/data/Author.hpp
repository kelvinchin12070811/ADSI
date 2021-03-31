/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <cstdint>
#include <string>

namespace db::data {
/**
 * @brief Author table of database
 * 
 * Describe the information of a author.
 */
struct Author
{
    Author() { }
    /**
     * @brief Construct new Author info from existing record.
     * @param authorID Unique ID of the author.
     * @param authorName Author name.
     * @param authorEmail Author email address.
     * @param authorPortFolioURL Author portfolio URL.
     */
    Author(std::uint32_t authorID, std::string authorName, std::string authorEmail,
           std::string authorPortFolioURL)
        : authorID { authorID },
          authorName { authorName },
          authorEmail { authorEmail },
          authorPortFolioURL { authorPortFolioURL }
    {
    }
    
    /**
     * @brief Construct new Author info entry to insert into database. Author ID will be given automatically.
     * @param authorName Author name.
     * @param authorEmail Author email address. 
     * @param authorPortFolioURL Author portfolio URL.
     */
    Author(std::string authorName, std::string authorEmail, std::string authorPortFolioURL)
        : Author(-1u, authorName, authorEmail, authorPortFolioURL)
    {
    }

    /**
     * @brief Unique ID of the Author record.
     */
    std::uint32_t authorID;
    /**
     * @brief Name of the author.
     */
    std::string authorName;
    /**
     * @brief Email of the author.
     */
    std::string authorEmail;
    /**
     * @brief Author's portfolio URL.
     */
    std::string authorPortFolioURL;
};
}