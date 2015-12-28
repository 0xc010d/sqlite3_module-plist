#include <numeric>
#include "PlistTable.hpp"

bool PlistTable::load(const std::string &path, int depth, const std::string &keyPath)
{
  auto plist = Plist::parse(path, keyPath);
  return load(plist, depth);
}

bool PlistTable::load(const void *buffer, const size_t size, int depth)
{
  auto plist = Plist::parse(buffer, size);
  return load(plist, depth);
}

bool PlistTable::load(const Plist &plist, int depth)
{
  if (!plist.isValid()) {
    return false;
  }

  m_table = getTable(plist, depth == 0 ? INT_MAX : depth);
  m_fields = m_table.getFields();
  return true;
}

Table<Cell> PlistTable::getTable(const Plist &plist, int depth, const std::string &prefix)
{
  if (plist.isRow()) {
    return getRowTable(plist.rowValue(), depth, prefix);
  }
  if (plist.isColumn()) {
    return getColumnTable(plist.columnValue(), depth, prefix);
  }
  const auto name = prefix.empty() ? "_" : prefix;
  return {name, plist};
}

Table<Cell> PlistTable::getRowTable(const Cell::Row &row, int depth, const std::string &prefix)
{
  /*
   * field names are taken from the dictionary keys
   * if prefix is not empty (e.g. it's not a root dictionary), field name is set to `prefix + "." + key`
   * if prefix is not empty and the key is empty (might happen), field name is set to `prefix`
   * if it's a root dictionary and the key is empty, field name is set to "_"
   */
  Table<Cell> table;
  if (depth-- == 0) return table;

  for (auto &item : row) {
    const auto delimiter = prefix.empty() || item.first.empty() ? "" : ".";
    const auto name = prefix.empty() && item.first.empty() ? "_" : prefix + delimiter + item.first;
    auto itemTable = getTable(item.second, depth, name);
    table.combine(itemTable);
  }

  return table;
}

Table<Cell> PlistTable::getColumnTable(const Cell::Column &column, int depth, const std::string &prefix, const size_t level)
{
  /*
   * field name is, by default, prefix
   * if prefix is empty (e.g. it's a root array), field name is set to "_"
   * in case of multilevel arrays ([1, 2, [3, 4]]), first level's name is prefix, subsequent levels have the name `prefix + "._"`
   */
  Table<Cell> table;
  if (depth-- == 0) return table;

  const auto name = prefix.empty() ? "_" : level == 0 ? prefix : prefix + "._";
  for (auto &item : column) {
    auto itemTable = item.isColumn() ?
                     getColumnTable(item.columnValue(), depth, name, level + 1) : //subsequent levels support
                     getTable(item, depth, item.isPrimitive() ? name : prefix);
    table.join(itemTable);
  }
  return table;
}
