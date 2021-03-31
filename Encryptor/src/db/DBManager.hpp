/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#pragma once
#include <sqlite_orm/sqlite_orm.h>

#include "db/data/KeyStore.hpp"
#include "db/data/Author.hpp"

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
                           make_column("keyPublic", &data::KeyStore::keyPublic),
                           make_column("keyPrivate", &data::KeyStore::keyPrivate),
                           make_column("keyPasswordHash", &data::KeyStore::keyPasswordHash),
                           foreign_key(&data::KeyStore::authorID)
                                   .references(&data::Author::authorID)));
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
     * @brief Get the connected database.
     * @return Connected SQLite3 database.
     */
    decltype(DBManager::createStorage()) &storage();

private:
    DBManager() {};

private:
    /**
     * @brief SQLite3 database connection hook.
     */
    decltype(DBManager::createStorage()) storage_ { createStorage() };
};
}