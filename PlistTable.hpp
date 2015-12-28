#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>
#include "Plist.hpp"
#include "Table.hpp"

class PlistTable
{
public:
  bool load(const std::string &, int, const std::string &);
  bool load(const void *, const size_t, int);

  const std::vector<std::string> &getFields() const { return m_fields; }
  const Cell &getCell(const int row, const int column) const { return m_table[m_fields[column]][row]; }
  size_t getHeight() const { return m_table.getHeight(); }

  sqlite3_vtab *getRef() { return &m_vtab; }

private:
  sqlite3_vtab m_vtab;

  bool load(const Plist &, int);

  static Table<Cell> getTable(const Plist &, int, const std::string & = "");
  static Table<Cell> getRowTable(const Cell::Row &, int, const std::string &);
  static Table<Cell> getColumnTable(const Cell::Column &, int, const std::string &, const size_t = 0);

  Table<Cell> m_table;
  std::vector<std::string> m_fields;
};
