#include <sys/cdefs.h>

#pragma once

#include <sqlite3.h>
#include "Cell.hpp"

class PlistCursor
{
public:
  PlistCursor(sqlite3_vtab *pVTab) { m_cursor.pVtab = pVTab; };

  sqlite3_vtab_cursor *getRef() { return &m_cursor; }

  void rewind();

  void next();

  bool eof() const;

  int getRowId() const;

  const Cell &getCell(const int column);

private:
  sqlite3_vtab_cursor m_cursor;

  size_t m_row = 0;
};
