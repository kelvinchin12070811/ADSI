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
     * @param keyPublic Base64 encoded author's RSA Public key encrypted with AES.
     * @param keyPrivate Base64 encoded author's RSA Private key encrypted with AES.
     * @param keyPasswordHash BCrypt hashed author's password use to encrypt data in the tuple.
     */
    KeyStore(std::uint32_t keyID, std::uint32_t authorID, std::string keyPublic,
             std::string keyPrivate, std::string keyPasswordHash)
        : keyID { keyID },
          authorID { authorID },
          keyPublic { keyPublic },
          keyPrivate { keyPrivate },
          keyPasswordHash { keyPasswordHash }
    {
    }
    
    /**
     * @brief Construct KeyStore object from existing data.
     * @param authorID Unique ID of the author referenced as db::data::Author.
     * @param keyPublic Base64 encoded author's RSA Public key encrypted with AES.
     * @param keyPrivate Base64 encoded author's RSA Private key encrypted with AES.
     * @param keyPasswordHash BCrypt hashed author's password use to encrypt data in the tuple.
     */
    KeyStore(std::uint32_t authorID, std::string keyPublic,
             std::string keyPrivate, std::string keyPasswordHash)
        : keyID { -1u },
          authorID { authorID },
          keyPublic { keyPublic },
          keyPrivate { keyPrivate },
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
     * @brief AES encrypted RSA Public key of the Author in base64.
     */
    std::string keyPublic;
    /**
     * @brief AES encrypted RSA Private key of the Author in base64.
     */
    std::string keyPrivate;
    /**
     * @brief BCrypt hashed password where Author use to encrypt his keys.
     */
    std::string keyPasswordHash;
};
}