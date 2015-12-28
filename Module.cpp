#include <stddef.h>
#include <sstream>

#include "Module.h"
#include "PlistTable.hpp"
#include "PlistCursor.hpp"

#include <sqlite3ext.h>

SQLITE_EXTENSION_INIT3

template<typename... Args>
static int ReportSQLiteError(char **pzErr, Args... args)
{
  if (*pzErr != NULL) {
    sqlite3_free(pzErr);
  }
  *pzErr = sqlite3_mprintf(args...);
  return SQLITE_ERROR;
}

int xConnect(sqlite3 *db, void *, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr)
{
  if (argc < 4) {
    return ReportSQLiteError(pzErr, "Please provide plist file path");
  }

  PlistTable *table = new PlistTable();

  int depth = argc > 4 ? atoi(argv[4]) : 0;
  std::string keyPath = argc > 5 ? argv[5] : "";

  if (!table->load(argv[3], depth, keyPath)) {
    return ReportSQLiteError(pzErr, "Failed loading plist from '%s'", argv[3]);
  }

  auto &fields = table->getFields();
  std::stringstream schema;
  schema << "CREATE TABLE x(";
  for (auto it = fields.begin(); it != fields.end(); ++it) {
    if (it != fields.begin())
      schema << ", ";
    schema << "\"" << *it << "\"";
  }
  schema << ")";
  auto result = sqlite3_declare_vtab(db, schema.str().c_str());
  if (result == SQLITE_OK) {
    *ppVTab = table->getRef();
  }
  return result;
}

int xDisconnect(sqlite3_vtab *pVTab)
{
  PlistTable *table = reinterpret_cast<PlistTable *>(pVTab);
  if (table != nullptr) {
    delete table;
  }
  return SQLITE_OK;
}

int xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor)
{
  auto cursor = new PlistCursor(pVTab);
  *ppCursor = cursor->getRef();
  return SQLITE_OK;
}

int xClose(sqlite3_vtab_cursor *pCursor)
{
  PlistCursor *cursor = reinterpret_cast<PlistCursor *>(pCursor);
  if (cursor != nullptr) {
    delete cursor;
  }
  return SQLITE_OK;
}

int xBestIndex(sqlite3_vtab *, sqlite3_index_info *)
{
  return SQLITE_OK;
}

int xFilter(sqlite3_vtab_cursor *pCursor, int, const char *, int, sqlite3_value **)
{
  PlistCursor *cursor = reinterpret_cast<PlistCursor *>(pCursor);
  cursor->rewind();
  return SQLITE_OK;
}

int xNext(sqlite3_vtab_cursor *pCursor)
{
  PlistCursor *cursor = reinterpret_cast<PlistCursor *>(pCursor);
  cursor->next();
  return SQLITE_OK;
}

int xEof(sqlite3_vtab_cursor *pCursor)
{
  PlistCursor *cursor = reinterpret_cast<PlistCursor *>(pCursor);
  return cursor->eof();
}

int xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *sqlite3, int n)
{
  PlistCursor *cursor = reinterpret_cast<PlistCursor *>(pCursor);
  auto &cell = cursor->getCell(n);
  switch (cell.type()) {
    case Cell::TEXT: {
      auto &text = cell.textValue();
      size_t size = text.size();
      char *buffer = (char *)malloc(size);
      memcpy(buffer, text.c_str(), size);
      sqlite3_result_text(sqlite3, buffer, (int)size, free);
      break;
    }
    case Cell::INTEGER:
      sqlite3_result_int64(sqlite3, cell.integerValue());
      break;
    case Cell::REAL:
      sqlite3_result_double(sqlite3, cell.realValue());
      break;
    case Cell::BLOB: {
      auto &blob = cell.blobValue();
      size_t size = blob.size();
      char *buffer = (char *)malloc(size);
      memcpy(buffer, blob.data(), size);
      sqlite3_result_blob(sqlite3, buffer, (int)size, free);
      break;
    }
    default:
      break;
  }
  return SQLITE_OK;
}

int xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid)
{
  PlistCursor *cursor = reinterpret_cast<PlistCursor *>(pCursor);
  *pRowid = cursor->getRowId();
  return SQLITE_OK;
}

int xRename(sqlite3_vtab *, const char *)
{
  return SQLITE_OK;
}

int registerModule(sqlite3 *db, const char *name)
{
  static const struct sqlite3_module module
    {
      .iVersion = 1,
      .xCreate = xConnect,
      .xConnect = xConnect,
      .xDisconnect = xDisconnect,
      .xDestroy = xDisconnect,
      .xOpen = xOpen,
      .xClose = xClose,
      .xBestIndex = xBestIndex,
      .xFilter = xFilter,
      .xNext = xNext,
      .xEof = xEof,
      .xColumn = xColumn,
      .xRowid = xRowid,
      .xRename = xRename,
    };
  return sqlite3_create_module(db, name, &module, NULL);
}
