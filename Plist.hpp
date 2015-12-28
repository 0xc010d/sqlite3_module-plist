#pragma once

#include "Cell.hpp"

class Plist : public Cell
{
public:
  static Plist parse(const std::string &path, const std::string &keyPath = "");
  static Plist parse(const void *buffer, size_t size, const std::string &keyPath = "");
  static Plist parse(CFPropertyListRef plist, const std::string &keyPath = "");

  Plist(const Cell &cell) : Cell(cell) { }
};
