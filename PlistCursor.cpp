#include "PlistCursor.hpp"
#include "PlistTable.hpp"

void PlistCursor::rewind()
{
  m_row = 0;
}

void PlistCursor::next()
{
  m_row++;
}

bool PlistCursor::eof() const
{
  return m_row == reinterpret_cast<PlistTable *>(m_cursor.pVtab)->getHeight();
}

int PlistCursor::getRowId() const
{
  return (int)m_row;
}

const Cell &PlistCursor::getCell(const int column)
{
  return reinterpret_cast<PlistTable *>(m_cursor.pVtab)->getCell((const int)m_row, column);
}
