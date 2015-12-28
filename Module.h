#pragma once

#include <sqlite3.h>

int registerModule(sqlite3 *db, const char *name);
