#include "DBManager.hpp"
/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QDebug>

#include <vector>

#include "db/DBManager.hpp"

namespace db {
DBManager &DBManager::getInstance()
{
    static DBManager instance;
    return instance;
}

void DBManager::initDB()
{
    storage_.sync_schema();
}

std::optional<data::Author> DBManager::getAuthorByID(std::uint32_t id)
{
    try {
        return storage_.get<data::Author>(id);
    } catch (const std::exception &e) {
        return std::nullopt;
    }
}

std::optional<data::Author> DBManager::getAuthorByName(std::string_view name)
{
    using namespace sqlite_orm;
    auto authors = storage_.get_all<data::Author>(
            where(c(&data::Author::authorName) == static_cast<std::string>(name)),
            limit(1));
    return authors.empty() ? std::nullopt : std::make_optional(*authors.begin());
}

std::optional<data::Author> DBManager::getAuthorByDistance(std::uint32_t distance)
{
    using namespace sqlite_orm;
    auto authors = storage_.get_all<data::Author>(order_by(&data::Author::authorID).desc(),
                                                  limit(1, offset(distance)));
    return authors.empty() ? std::nullopt : std::make_optional(*authors.begin());
}

std::uint32_t DBManager::insertNewAuthor(const data::Author &author)
{
    return storage_.insert(author);
}

void DBManager::updateAuthor(const data::Author &author)
{
    storage_.update(author);
}

void DBManager::removeAuthorById(std::uint32_t id)
{
    storage_.remove<data::Author>(id);
}

void DBManager::insertNewKeyForAuthor(const data::KeyStore &key)
{
    storage_.insert(key);
}

void DBManager::removeKeyByID(std::uint32_t id)
{
    storage_.remove<data::KeyStore>(id);
}

std::optional<data::KeyStore> DBManager::getAuthorKeyByDistance(std::uint32_t authorID,
                                                                uint32_t distance)
{
    using namespace sqlite_orm;
    auto key = storage_.get_all<data::KeyStore>(where(c(&data::KeyStore::authorID) == authorID),
                                                order_by(&data::KeyStore::keyID),
                                                limit(1, distance));
    return key.empty() ? std::nullopt : std::make_optional(*key.begin());
}
}

