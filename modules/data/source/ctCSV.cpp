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
    m_cells.emplace_back();
    m_cells.back().reserve(cols.size());
    for (ctString &val : cols)
      m_cells.back().emplace_back(val.trim(cellTrimChars));
  }

  return true;
}

ctStringValue ctCSV::Get(const int64_t &row, const int64_t &column) const
{
  const ctStringValue *pVal = TryGetValue(row, column);
  return pVal ? *pVal : ctStringValue();
}

ctType ctCSV::GetType(const int64_t &row, const int64_t &column) const
{
  const ctStringValue *pVal = TryGetValue(row, column);
  return pVal ? pVal->GetType() : ctType_Unknown;
}

int64_t ctCSV::AsInt(const int64_t &row, const int64_t &column) const
{
  const ctStringValue *pVal = TryGetValue(row, column);
  return pVal ? pVal->AsInt() : 0;
}

bool ctCSV::AsBool(const int64_t &row, const int64_t &column) const
{
  const ctStringValue *pVal = TryGetValue(row, column);
  return pVal ? pVal->AsBool() : 0;
}

double ctCSV::AsFloat(const int64_t &row, const int64_t &column) const
{
  const ctStringValue *pVal = TryGetValue(row, column);
  return pVal ? pVal->AsFloat() : 0;
}

ctString ctCSV::AsString(const int64_t &row, const int64_t &column) const
{
  const ctStringValue *pVal = TryGetValue(row, column);
  return pVal ? pVal->AsString() : "";
}

bool ctCSV::IsEmpty(const int64_t &row, const int64_t &column) const
{
  const ctStringValue *pVal = TryGetValue(row, column);
  return !pVal || pVal->IsEmpty();
}

void ctCSV::Set(const ctStringValue &value, const int64_t &row, const int64_t &col)
{
  ctStringValue *pCell = TryGetValue(row, col);
  if (pCell)
    *pCell = value;
  else
    Add(value, row, col);
}

bool ctCSV::Add(const ctStringValue &value, const int64_t &row, const int64_t &col)
{
  ctStringValue *pCell = TryGetValue(row, col);
  if (pCell && !pCell->IsEmpty())
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

const ctStringValue* ctCSV::TryGetValue(const int64_t &row, const int64_t &col) const { return col >= 0 && col < GetColCount(row) ? &m_cells[row][col] : nullptr; }
ctStringValue * ctCSV::TryGetValue(const int64_t &row, const int64_t &col) { return col >= 0 && col < GetColCount(row) ? &m_cells[row][col] : nullptr; }
int64_t ctCSV::GetRowCount() const { return m_cells.size(); }
int64_t ctCSV::GetColCount(const int64_t &row) const { return row >= 0 && row < GetRowCount() ? m_cells[row].size() : 0; }

ctString ctToString(const ctCSV &csv)
{
  ctVector<ctString> joinedRows;
  for (const ctVector<ctStringValue> &row : csv.m_cells)
  {
    ctString joined = "";
    for (int64_t i = 0; i < row.size(); ++i)
      joined += (i == 0 ? row[i] : ("," + row[i].AsString()));
    joinedRows.emplace_back(std::move(joined));
  }
  return ctString::join(joinedRows, "\n", false);
}
