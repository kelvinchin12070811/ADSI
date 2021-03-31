/**********************************************************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *********************************************************************************************************************/
#include <QDebug>

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

decltype(DBManager::createStorage()) &DBManager::storage()
{
    return storage_;
}
}