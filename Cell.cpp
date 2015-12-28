#include "Cell.hpp"

using std::nullptr_t;
using std::make_shared;

template<Cell::Type _type, typename T>
class TemplateCell : public ValueCell
{
protected:
  TemplateCell(const T &value) : m_value(value) { }
  Cell::Type type() const override { return _type; }
  const T m_value;
};

class RowCell : public TemplateCell<Cell::ROW, Cell::Row>
{
public:
  RowCell(const Cell::Row &value) : TemplateCell(value) { }
  virtual const Cell::Row &rowValue() const override { return m_value; }
  virtual const Cell &operator[](const Cell::Name &name) const override {
    auto it = m_value.find(name);
    return (it != m_value.end()) ? it->second : ValueCell::operator[](name);
  }
  virtual size_t size() const override { return m_value.size(); }
};

class ColumnCell : public TemplateCell<Cell::COLUMN, Cell::Column>
{
public:
  ColumnCell(const Cell::Column &value) : TemplateCell(value) { }
  virtual const Cell::Column &columnValue() const override { return m_value; };
  virtual const Cell &operator[](const Cell::Index &index) const override { return m_value[index]; }
  virtual size_t size() const override { return m_value.size(); }
};

class TextCell : public TemplateCell<Cell::TEXT, Cell::Text>
{
public:
  TextCell(const Cell::Text &value) : TemplateCell(value) { }
  virtual const Cell::Text &textValue() const override { return m_value; };
};

class IntegerCell : public TemplateCell<Cell::INTEGER, Cell::Integer>
{
public:
  IntegerCell(const Cell::Integer &value) : TemplateCell(value) { }
  virtual const Cell::Integer &integerValue() const override { return m_value; };
};

class RealCell : public TemplateCell<Cell::REAL, Cell::Real>
{
public:
  RealCell(const Cell::Real &value) : TemplateCell(value) { }
  virtual const Cell::Real &realValue() const override { return m_value; };
};

class BlobCell : public TemplateCell<Cell::BLOB, Cell::Blob>
{
public:
  BlobCell(const Cell::Blob &value) : TemplateCell(value) { }
  virtual const Cell::Blob &blobValue() const override { return m_value; };
};

class NullCell : public TemplateCell<Cell::NUL, nullptr_t>
{
public:
  NullCell(const nullptr_t &value) : TemplateCell(value) { }
};

Cell::Cell(const Cell::Row &row) : m_ptr(make_shared<RowCell>(row)) { }
Cell::Cell(const Cell::Column &column) : m_ptr(make_shared<ColumnCell>(column)) { }
Cell::Cell(const Cell::Text &text) : m_ptr(make_shared<TextCell>(text)) { }
Cell::Cell(const Cell::Integer &integer) : m_ptr(make_shared<IntegerCell>(integer)) { }
Cell::Cell(const Cell::Real &real) : m_ptr(make_shared<RealCell>(real)) { }
Cell::Cell(const Cell::Blob &blob) : m_ptr(make_shared<BlobCell>(blob)) { }
Cell::Cell(const nullptr_t &null) : m_ptr(make_shared<NullCell>(null)) { }

Cell::Type Cell::type() const { return m_ptr->type(); }

size_t Cell::size() const { return m_ptr->size(); }

size_t ValueCell::size() const { return 1; }

const Cell::Row &ValueCell::rowValue() const
{
  static const Cell::Row row;
  return row;
}

const Cell::Column &ValueCell::columnValue() const
{
  static const Cell::Column column;
  return column;
}

const Cell::Text &ValueCell::textValue() const
{
  static const Cell::Text text;
  return text;
}

const Cell::Integer &ValueCell::integerValue() const
{
  static const Cell::Integer integer = 0;
  return integer;
}

const Cell::Real &ValueCell::realValue() const
{
  static const Cell::Real real = 0;
  return real;
}

const Cell::Blob &ValueCell::blobValue() const
{
  static const Cell::Blob blob;
  return blob;
}

const Cell &ValueCell::operator[](const Cell::Index &) const
{
  static const Cell cell;
  return cell;
}

const Cell &ValueCell::operator[](const Cell::Name &) const
{
  static const Cell cell;
  return cell;
}

const Cell::Row &Cell::rowValue() const { return m_ptr->rowValue(); }
const Cell::Column &Cell::columnValue() const { return m_ptr->columnValue(); }
const Cell::Text &Cell::textValue() const { return m_ptr->textValue(); }
const Cell::Integer &Cell::integerValue() const { return m_ptr->integerValue(); }
const Cell::Real &Cell::realValue() const { return m_ptr->realValue(); }
const Cell::Blob &Cell::blobValue() const { return m_ptr->blobValue(); }
const Cell &Cell::operator[](const Cell::Index &index) const {return (*m_ptr)[index];}
const Cell &Cell::operator[](const Cell::Name &key) const {return (*m_ptr)[key];}

Cell _parse(CFDictionaryRef dictionaryRef)
{
  Cell::Row row;
  CFIndex size = CFDictionaryGetCount(dictionaryRef);
  CFStringRef keys[size];
  CFTypeRef values[size];
  CFDictionaryGetKeysAndValues(dictionaryRef, (const void **)keys, values);
  for (CFIndex index = 0; index < size; index++) {
    Cell::Name name;
    const char *stringPtr = CFStringGetCStringPtr(keys[index], kCFStringEncodingUTF8);
    if (stringPtr == NULL) {
      CFIndex length = CFStringGetLength(keys[index]);
      CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
      char buffer[maxSize];
      if (CFStringGetCString(keys[index], buffer, maxSize, kCFStringEncodingUTF8)) {
        name = buffer;
      }
    }
    else {
      name = stringPtr;
    }
    CFTypeRef value = values[index];
    auto cell = Cell::parse(value);
    row.insert({name, cell});
  }
  return row;
}

Cell _parse(CFArrayRef arrayRef)
{
  Cell::Column column;
  CFIndex count = CFArrayGetCount(arrayRef);
  for (CFIndex index = 0; index < count; index++) {
    CFTypeRef item = CFArrayGetValueAtIndex(arrayRef, index);
    auto cell = Cell::parse(item);
    column.push_back(cell);
  }
  return column;
}

Cell _parse(CFStringRef stringRef)
{
  const char *stringPtr = CFStringGetCStringPtr(stringRef, kCFStringEncodingUTF8);
  if (stringPtr != NULL) {
    return (Cell::Text)stringPtr;
  }
  CFIndex length = CFStringGetLength(stringRef);
  CFIndex size = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8);
  char buffer[size];
  if (CFStringGetCString(stringRef, buffer, size, kCFStringEncodingUTF8)) {
    return Cell::Text(buffer);
  }
  return Cell::Text();
}

Cell _parse(CFBooleanRef boolanRef)
{
  return (Cell::Integer)(CFBooleanGetValue(boolanRef));
}

Cell _parse(CFNumberRef numberRef)
{
  if (CFNumberIsFloatType(numberRef)) {
    Cell::Real real;
    CFNumberGetValue(numberRef, kCFNumberDoubleType, &real);
    return real;
  }
  Cell::Integer integer;
  CFNumberGetValue(numberRef, kCFNumberSInt64Type, &integer);
  return integer;
}

Cell _parse(CFDateRef dateRef)
{
  return CFDateGetAbsoluteTime(dateRef);
}

Cell _parse(CFDataRef dataRef)
{
  Cell::Blob blob;
  const UInt8 *buffer = CFDataGetBytePtr(dataRef);
  CFIndex length = CFDataGetLength(dataRef);
  blob.assign(buffer, buffer + length);
  return blob;
}

Cell Cell::parse(const CFTypeRef ref)
{
  if (ref == NULL) {
    return nullptr;
  }
  CFTypeID type = CFGetTypeID(ref);
  if (type == CFDictionaryGetTypeID()) {
    return _parse((CFDictionaryRef)ref);
  }
  if (type == CFArrayGetTypeID()) {
    return _parse((CFArrayRef)ref);
  }
  if (type == CFStringGetTypeID()) {
    return _parse((CFStringRef)ref);
  }
  if (type == CFBooleanGetTypeID()) {
    return _parse((CFBooleanRef)ref);
  }
  if (type == CFNumberGetTypeID()) {
    return _parse((CFNumberRef)ref);
  }
  if (type == CFDateGetTypeID()) {
    return _parse((CFDateRef)ref);
  }
  if (type == CFDataGetTypeID()) {
    return _parse((CFDataRef)ref);
  }
  return nullptr;
}
