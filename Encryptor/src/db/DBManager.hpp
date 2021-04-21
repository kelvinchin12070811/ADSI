/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <optional>
#include <string_view>

#include <sqlite_orm/sqlite_orm.h>

#include "db/data/Author.hpp"
#include "db/data/KeyStore.hpp"

namespace db {
/**
 * @brief Singleton object to manage database connection.
 * 
 * This singleton object help ADSI Encryptor to communicate with its SQLite3 database.
 */
class DBManager
{
public:
    /**
     * @brief Initializer function use to create connection to the SQLite3 database.
     * @tparam ...Args Represented as arguments passed into the template.
     * @return Connected SQLite3 database.
     */
    template<typename... Args>
    auto static createStorage()
    {
        using namespace sqlite_orm;

        return make_storage(
                "usrdata.db",
                make_table("Author",
                           make_column("authorID", &data::Author::authorID, primary_key(),
                                       autoincrement()),
                           make_column("authorName", &data::Author::authorName),
                           make_column("authorEmail", &data::Author::authorEmail),
                           make_column("authorPortFolioURL", &data::Author::authorPortFolioURL)),
                make_table("KeyStore",
                           make_column("keyID", &data::KeyStore::keyID, primary_key(),
                                       autoincrement()),
                           make_column("authorID", &data::KeyStore::authorID),
                           make_column("keyParams", &data::KeyStore::keyParams),
                           make_column("keyPasswordHash", &data::KeyStore::keyPasswordHash),
                           foreign_key(&data::KeyStore::authorID)
                                   .references(&data::Author::authorID)));
    }

    /**
     * @brief Get iterator to iterate trough all available authors.
     * @return Iterator to access all authors.
     */
    auto iterateAuthor()
    {
        return storage_.iterate<data::Author>(sqlite_orm::order_by(&data::Author::authorID).desc());
    };

    auto iterateKeysByAuthor(std::uint32_t authorID)
    {
        using namespace sqlite_orm;
        return storage_.iterate<data::KeyStore>(where(c(&data::KeyStore::authorID) == authorID),
                                                order_by(&data::KeyStore::keyID));
    }

public:
    DBManager(const DBManager &) = delete;
    DBManager(DBManager &&) = delete;
    DBManager &operator=(const DBManager &) = delete;
    DBManager &operator=(DBManager &&) = delete;

    /**
     * @brief Get unique instance of the DBManager.
     * @return Unique instance of the DBManager.
     */
    static DBManager &getInstance();

    /**
     * @brief Initialize the SQLite3 database.
     */
    void initDB();
    /**
     * @brief Get author by it's unique id.
     * @param id ID of the author to retrieve.
     * @return Author with at the corresponded id, std::nullopt if author with @p id not found in database.
     */
    std::optional<data::Author> getAuthorByID(std::uint32_t id);
    /**
     * @brief Get author by it's name.
     * @param name Name of the author to get.
     * @return Author with name of @p name, std::nullopt if author with @p name not found in database.
     */
    std::optional<data::Author> getAuthorByName(std::string_view name);
    /**
     * @brief Get author at the specified distance.
     * @param distance Dinstance of the entry from begin.
     * @return Author at specified distance, std::nullopt if author with @p distance not found in database.
     */
    std::optional<data::Author> getAuthorByDistance(std::uint32_t distance);
    /**
     * @brief Insert new author into database.
     * @param author Author to insert.
     * @return Index of the author record newly created.
     */
    std::uint32_t insertNewAuthor(const data::Author &author);
    /**
     * @brief Update author entry.
     * @param author Author data to update.
     */
    void updateAuthor(const data::Author &author);
    /**
     * @brief Remove author by its id.
     * @param id ID of author to remove.
     */
    void removeAuthorById(std::uint32_t id);
    /**
     * @brief Insert newly generated key for author into database.
     * @param key Key object to insert.
     */
    void insertNewKeyForAuthor(const data::KeyStore &key);
    /**
     * @brief Remove selected key from database.
     * @param id Key object to remove.
     */
    void removeKeyByID(std::uint32_t id);
    /**
     * @brief Get key of author at the specified distance.
     * @param authorID ID of the author to query.
     * @param distance Distance of the entry from begin.
     * @return Author's key at specified distance, std::nullopt if no key found.
     */
    std::optional<data::KeyStore> getAuthorKeyByDistance(std::uint32_t authorID, std::uint32_t distance);

private:
    DBManager() {};

private:
    /**
     * @brief SQLite3 database connection hook.
     */
    decltype(DBManager::createStorage()) storage_ { createStorage() };
};
}