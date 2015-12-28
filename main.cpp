#include <iostream>
#include <sqlite3ext.h>
#include <config.h>

#include "Module.h"

SQLITE_EXTENSION_INIT1

#ifndef SQLITE3_EXTENSION_INIT_FUNCTION
#ifndef SQLITE3_EXTENSION_NAME
#define SQLITE3_EXTENSION_NAME extension
#endif
#define MAKE_INIT_FUNCTION(module) sqlite3_ ## module ## _init
#define INIT_FUNCTION(module) MAKE_INIT_FUNCTION(module)
#define SQLITE3_EXTENSION_INIT_FUNCTION INIT_FUNCTION(SQLITE3_EXTENSION_NAME)
#endif

extern "C" {
__attribute__((used))
int SQLITE3_EXTENSION_INIT_FUNCTION(sqlite3 *db, char __unused **pzErrMsg, const sqlite3_api_routines *pApi)
{
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  registerModule(db, SQLITE3_MODULE_NAME);
  return rc;
}
}
