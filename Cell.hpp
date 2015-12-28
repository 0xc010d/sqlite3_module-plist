#pragma once

#include <map>
#include <string>
#include <vector>
#include <CoreFoundation/CoreFoundation.h>
#include <set>

class ValueCell;

class Cell
{
public:
  enum Type
  {
    ROW, COLUMN, TEXT, INTEGER, REAL, BLOB, NUL
  };

  typedef std::string Name;
  typedef size_t Index;
  typedef std::map<Name, Cell> Row;
  typedef std::vector<Cell> Column;
  typedef std::string Text;
  typedef int64_t Integer;
  typedef double Real;
  typedef std::vector<uint8_t> Blob;

  Cell(const Row &);
  Cell(const Column &);
  Cell(const Text &);
  Cell(const Integer &);
  Cell(const Real &);
  Cell(const Blob &);

  Cell(const std::nullptr_t &);
  Cell() : Cell(nullptr) {};

  Type type() const;

  size_t size() const;

  bool isRow() const { return type() == ROW; }
  bool isColumn() const { return type() == COLUMN; }
  bool isText() const { return type() == TEXT; }
  bool isInteger() const { return type() == INTEGER; }
  bool isReal() const { return type() == REAL; }
  bool isBlob() const { return type() == BLOB; }
  bool isNull() const { return type() == NUL; }

  bool isValid() const { return !isNull(); }
  bool isPrimitive() const { return type() != ROW && type() != COLUMN; };

  const Row &rowValue() const;
  const Column &columnValue() const;
  const Text &textValue() const;
  const Integer &integerValue() const;
  const Real &realValue() const;
  const Blob &blobValue() const;

  const Cell &operator[](const Index &) const;
  const Cell &operator[](const Name &) const;

  static Cell parse(const CFTypeRef);

private:
  std::shared_ptr<ValueCell> m_ptr;
};

class ValueCell
{
protected:
  friend class Cell;

  virtual Cell::Type type() const = 0;

  virtual size_t size() const;

  virtual const Cell::Row &rowValue() const;
  virtual const Cell::Column &columnValue() const;
  virtual const Cell::Text &textValue() const;
  virtual const Cell::Integer &integerValue() const;
  virtual const Cell::Real &realValue() const;
  virtual const Cell::Blob &blobValue() const;

  virtual const Cell &operator[](const Cell::Index &) const;
  virtual const Cell &operator[](const Cell::Name &) const;

  virtual ~ValueCell() { }
};
