#include <gtest/gtest.h>
#include <Cell.hpp>

TEST(Cell, Null)
{
  Cell cell;
  ASSERT_EQ(cell.isValid(), false);
  ASSERT_EQ(cell.isRow(), false);
  ASSERT_EQ(cell.isColumn(), false);
  ASSERT_EQ(cell.isText(), false);
  ASSERT_EQ(cell.isInteger(), false);
  ASSERT_EQ(cell.isReal(), false);
  ASSERT_EQ(cell.isBlob(), false);
}

TEST(Cell, BooleanTrue)
{
  auto cell = Cell::parse(kCFBooleanTrue);
  ASSERT_EQ(cell.isInteger(), true);
  ASSERT_EQ(cell.integerValue(), 1);
}

TEST(Cell, BooleanFalse)
{
  auto cell = Cell::parse(kCFBooleanFalse);
  ASSERT_EQ(cell.isInteger(), true);
  ASSERT_EQ(cell.integerValue(), 0);
}

TEST(Cell, StringEmpty)
{
  auto cell = Cell::parse(CFSTR(""));
  ASSERT_EQ(cell.isText(), true);
  ASSERT_EQ(cell.textValue(), "");
}

TEST(Cell, StringNonempty)
{
  auto cell = Cell::parse(CFSTR("text"));
  ASSERT_EQ(cell.isText(), true);
  ASSERT_EQ(cell.textValue(), "text");
}

TEST(Cell, NumberByte)
{
  int8_t value = -1;
  CFNumberRef number = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt8Type, &value);
  auto cell = Cell::parse(number);
  CFRelease(number);
  ASSERT_EQ(cell.isInteger(), true);
  ASSERT_EQ(cell.integerValue(), value);
}

TEST(Cell, NumberLongLong)
{
  int64_t value = -655392;
  CFNumberRef number = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt64Type, &value);
  auto cell = Cell::parse(number);
  CFRelease(number);
  ASSERT_EQ(cell.isInteger(), true);
  ASSERT_EQ(cell.integerValue(), value);
}

TEST(Cell, NumberFloat)
{
  float value = -2;
  CFNumberRef number = CFNumberCreate(kCFAllocatorDefault, kCFNumberFloat32Type, &value);
  auto cell = Cell::parse(number);
  CFRelease(number);
  ASSERT_EQ(cell.isReal(), true);
  ASSERT_EQ(cell.realValue(), value);
}

TEST(Cell, NumberDouble)
{
  double value = 3.14;
  CFNumberRef number = CFNumberCreate(kCFAllocatorDefault, kCFNumberFloat64Type, &value);
  auto cell = Cell::parse(number);
  CFRelease(number);
  ASSERT_EQ(cell.isReal(), true);
  ASSERT_EQ(cell.realValue(), value);
}

TEST(Cell, DateZero)
{
  CFDateRef date = CFDateCreate(kCFAllocatorDefault, 0);
  auto cell = Cell::parse(date);
  CFRelease(date);
  ASSERT_EQ(cell.isReal(), true);
  ASSERT_EQ(cell.realValue(), 0);
}

TEST(Cell, DateInt)
{
  CFDateRef date = CFDateCreate(kCFAllocatorDefault, 65535);
  auto cell = Cell::parse(date);
  CFRelease(date);
  ASSERT_EQ(cell.isReal(), true);
  ASSERT_EQ(cell.realValue(), 65535);
}

TEST(Cell, DateFloat)
{
  CFDateRef date = CFDateCreate(kCFAllocatorDefault, 0.2);
  auto cell = Cell::parse(date);
  CFRelease(date);
  ASSERT_EQ(cell.isReal(), true);
  ASSERT_EQ(cell.realValue(), 0.2);
}
