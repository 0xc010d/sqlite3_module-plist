#pragma once

#include <vector>
#include "Cell.hpp"

template<typename T>
class Table
{
public:
  Table() = default;

  Table(const std::string &field, const T &value) : m_table{{field, {value}}}, m_height{1}, m_fields{{field}} { }

  Table(const std::map<std::string, std::vector<T>> &table) : m_table{table}
  {
    if (table.size() != 0) {
      m_height = table.begin()->second.size();
    }
    for (auto &it : table) {
      m_fields.push_back(it.first);
    }
  }

  const size_t &getHeight() const { return m_height; }

  const std::vector<std::string> &getFields() const { return m_fields; }

  const std::map<std::string, std::vector<T>> &getTable() const { return m_table; }

  /*
   *                                         a   b   c   aa   bb
   *                                       -----------------------
   *         a   b   c                     | 1 | 2 | 3 | -- | -- |
   *       -------------     aa   bb       -----------------------
   *       | 1 | 2 | 3 |   -----------     | 4 | 5 | 6 | -- | -- |
   *       -------------   | 11 | 12 |     -----------------------
   * Join( | 4 | 5 | 6 | , ----------- ) = | 7 | 8 | 9 | -- | -- |
   *       -------------   | 13 | 14 |     -----------------------
   *       | 7 | 8 | 9 |   -----------     | - | - | - | 11 | 12 |
   *       -------------                   -----------------------
   *                                       | - | - | - | 13 | 14 |
   *                                       -----------------------
   *
   *                                          a   b   c   bb
   *                                       -------------------
   *         a   b   c                     |  1 | 2 | 3 | -- |
   *       -------------      a   bb       -------------------
   *       | 1 | 2 | 3 |   -----------     |  4 | 5 | 6 | -- |
   *       -------------   | 11 | 12 |     -------------------
   * Join( | 4 | 5 | 6 | , ----------- ) = |  7 | 8 | 9 | -- |
   *       -------------   | 13 | 14 |     -------------------
   *       | 7 | 8 | 9 |   -----------     | 11 | - | - | 12 |
   *       -------------                   -------------------
   *                                       | 13 | - | - | 14 |
   *                                       -------------------
   */
  void join(const Table<T> &other)
  {
    static const T defaultValue = {};
    for (auto &it : m_table) {
      it.second.resize(m_height + other.m_height);
    }
    for (auto &it : other.m_table) {
      auto &column = m_table[it.first];
      column.resize(m_height, defaultValue);
      column.insert(column.end(), it.second.begin(), it.second.end());
      if (std::find(m_fields.begin(), m_fields.end(), it.first) == m_fields.end()) {
        m_fields.push_back(it.first);
      }
    }
    m_height += other.m_height;
  }

  /*
   *                                            a   b   c   aa   bb
   *                                          -----------------------
   *                                          | 1 | 2 | 3 | 11 | 12 |
   *            a   b   c                     -----------------------
   *          -------------     aa   bb       | 4 | 5 | 6 | 11 | 12 |
   *          | 1 | 2 | 3 |   -----------     -----------------------
   *          -------------   | 11 | 12 |     | 7 | 8 | 9 | 11 | 12 |
   * Combine( | 4 | 5 | 6 | , ----------- ) = -----------------------
   *          -------------   | 13 | 14 |     | 1 | 2 | 3 | 13 | 14 |
   *          | 7 | 8 | 9 |   -----------     -----------------------
   *          -------------                   | 4 | 5 | 6 | 13 | 14 |
   *                                          -----------------------
   *                                          | 7 | 8 | 9 | 13 | 14 |
   *                                          -----------------------
   *
   * this method, unlike `join`, is not intended to work with tables that 'share' columns
   */
  void combine(const Table<T> &other)
  {
    if (other.m_height == 0) {
      return;
    }
    if (m_height == 0) {
      m_table = other.m_table;
      m_height = other.m_height;
      m_fields = other.m_fields;
      return;
    }

    for (auto &it : m_table) {
      auto &column = it.second;
      for (size_t i = 1; i < other.m_height; i++) {
        column.insert(column.end(), column.begin(), column.begin() + m_height);
      }
    }
    for (auto &it : other.m_table) {
      auto &column = m_table[it.first];
      std::for_each(it.second.begin(), it.second.end(), [&](const T &item) { column.insert(column.end(), m_height, item); });
      m_fields.push_back(it.first);
    }

    m_height *= other.m_height;
  }

  const std::vector<T> &operator[](const std::string &field) const
  {
    if (m_table.find(field) != m_table.end()) {
      return m_table.at(field);
    }
    static const std::vector<T> defaultValue = {};
    return defaultValue;
  }

private:
  std::map<std::string, std::vector<T>> m_table;
  size_t m_height = 0;
  std::vector<std::string> m_fields;
};
