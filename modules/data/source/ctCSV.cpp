#include "ctCSV.h"
#include "ctScan.h"

ctCSV::ctCSV(const ctString &csv) { Parse(csv); }
ctCSV::ctCSV(ctCSV &&csv) { *this = std::move(csv); }
ctCSV::ctCSV(const ctCSV &csv) { *this = csv; }

ctCSV& ctCSV::operator=(ctCSV &&rhs)
{
  std::swap(m_cells, rhs.m_cells);
  return *this;
}

ctCSV& ctCSV::operator=(const ctCSV &rhs)
{
  m_cells = rhs.m_cells;
  return *this;
}

bool ctCSV::Parse(const ctString &csv)
{
  *this = ctCSV();

  if (csv.length() == 0)
    return false;

  ctString cellTrimChars = ctString("\"") + ctString::Whitespace();
  // Split by row
  ctVector<ctString> rows = csv.split("\r\n", true);
  m_cells.reserve(rows.size());

  for (const ctString &row : rows)
  {
    // Rows by columns
    ctVector<ctString> cols = row.trim(ctString::Whitespace()).split(',', false);

    for (ctString &val : cols)
      val = val.trim(cellTrimChars);

    m_cells.push_back(std::move(cols));
  }

  return true;
}

int64_t ctCSV::AsInt(const int64_t &row, const int64_t &column) const
{
  const ctString *pVal = TryGetValue(row, column);
  return pVal ? ctScan::Int(*pVal) : 0;
}

bool ctCSV::AsBool(const int64_t &row, const int64_t &column) const
{
  const ctString *pVal = TryGetValue(row, column);
  return pVal ? ctScan::Bool(*pVal) : 0;
}

double ctCSV::AsFloat(const int64_t &row, const int64_t &column) const
{
  const ctString *pVal = TryGetValue(row, column);
  return pVal ? ctScan::Float(*pVal) : 0;
}

ctString ctCSV::AsString(const int64_t &row, const int64_t &column) const
{
  const ctString *pVal = TryGetValue(row, column);
  return pVal ? *pVal : "";
}

void ctCSV::SetValue(const ctString &value, const int64_t &row, const int64_t &col)
{
  ctString *pCell = TryGetValue(row, col);
  if (pCell)
    *pCell = value;
  else
    AddValue(value, row, col);
}

bool ctCSV::AddValue(const ctString &value, const int64_t &row, const int64_t &col)
{
  ctString *pCell = TryGetValue(row, col);
  if (pCell && pCell->length() > 0)
    return false;

  if (!pCell)
  {
    m_cells.resize(ctMax(m_cells.size(), row + 1));
    m_cells[row].resize(ctMax(m_cells[row].size(), col + 1));
    pCell = TryGetValue(row, col);

    if (!pCell)
      return false;
  }

  *pCell = value;
  return true;
}

const ctString* ctCSV::TryGetValue(const int64_t &row, const int64_t &col) const { return col >= 0 && col < GetColCount(row) ? &m_cells[row][col] : nullptr; }
ctString* ctCSV::TryGetValue(const int64_t &row, const int64_t &col) { return col >= 0 && col < GetColCount(row) ? &m_cells[row][col] : nullptr; }
int64_t ctCSV::GetRowCount() const { return m_cells.size(); }
int64_t ctCSV::GetColCount(const int64_t &row) const { return row >= 0 && row < GetRowCount() ? m_cells[row].size() : 0; }

ctString ctToString(const ctCSV &csv)
{
  ctVector<ctString> joinedRows;
  for (const ctVector<ctString> &row : csv.m_cells)
    joinedRows.push_back(ctString::join(row, ",", false));
  return ctString::join(joinedRows, "\n", false);
}
