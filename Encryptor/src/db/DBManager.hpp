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
class DBManager
{
public:
    DBManager(const DBManager &) = delete;
    DBManager(DBManager &&) = delete;
    DBManager &operator=(const DBManager &) = delete;
    DBManager &operator=(DBManager &&) = delete;

    static DBManager &getInstance();

    void initDB();

private:
    DBManager() {};

    template <typename... Args>
    auto static createStorage()
    {
        using namespace sqlite_orm;
        return make_storage(
                "bin.db",
                make_table("Author",
                           make_column("authorID", &data::Author::authorID, primary_key(),
                                       autoincrement()),
                           make_column("authorName", &data::Author::authorName),
                           make_column("authorEmail", &data::Author::authorEmail),
                           make_column("authorPortFolioURL", &data::Author::authorPortFolioURL)),
                make_table(
                        "KeyStore",
                        make_column("keyID", &data::KeyStore::keyID, primary_key(),
                                    autoincrement()),
                        foreign_key(&data::KeyStore::authorID).references(&data::Author::authorID),
                        make_column("keyPublic", &data::KeyStore::keyPublic),
                        make_column("keyPrivate", &data::KeyStore::keyPrivate),
                        make_column("keyPasswordHash", &data::KeyStore::keyPasswordHash)));
    }

private:
    decltype(createStorage()) storage { createStorage() };
};
}