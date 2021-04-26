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
 * @brief KeyStore table of the database.
 * 
 * This table store the keys that generated for the author including encrypted RSA encryption key and AES Encryption
 * password hash.
 */
struct KeyStore
{
    KeyStore() { }
    /**
     * @brief Construct KeyStore object from existing data.
     * @param keyID Unique ID of the KeyStore data.
     * @param authorID Unique ID of the author referenced as db::data::Author.
     * @param keyParams Base64 encoded author's RSA keys parameters encrypted with AES.
     * @param keyPasswordHash BCrypt hashed author's password use to encrypt data in the tuple.
     */
    KeyStore(std::uint32_t keyID, std::uint32_t authorID, std::string keyParams,
             std::string keyPasswordHash)
        : keyID { keyID },
          authorID { authorID },
          keyParams { keyParams },
          keyPasswordHash { keyPasswordHash }
    {
    }
    
    /**
     * @brief Construct KeyStore object from existing data.
     * @param authorID Unique ID of the author referenced as db::data::Author.
     * @param keyParams Base64 encoded author's RSA keys parameters encrypted with AES.
     * @param keyPasswordHash BCrypt hashed author's password use to encrypt data in the tuple.
     */
    KeyStore(std::uint32_t authorID, std::string keyParams, std::string keyPasswordHash)
        : keyID { -1u },
          authorID { authorID },
          keyParams { keyParams },
          keyPasswordHash { keyPasswordHash }
    {
    }

    /**
     * @brief Unique ID of the data column.
     */
    std::uint32_t keyID;
    /**
     * @brief Unique ID of the author represented in db::data::Author table.
     */
    std::uint32_t authorID;
    /**
     * @brief AES encrypted RSA Keys generation parameters.
     */
    std::string keyParams;
    /**
     * @brief BCrypt hashed password where Author use to encrypt his keys.
     */
    std::string keyPasswordHash;
};
}