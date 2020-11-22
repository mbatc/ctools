#ifndef atCSV_h__
#define atCSV_h__

#include "ctString.h"

class ctCSV
{
public:
  ctCSV() = default;

  ctCSV(const ctString &csv);

  ctCSV(ctCSV &&csv);
  ctCSV(const ctCSV &csv);

  ctCSV& operator=(ctCSV &&rhs);
  ctCSV& operator=(const ctCSV &rhs);

  bool Parse(const ctString &csv);

  int64_t AsInt(const int64_t &row, const int64_t &column) const;
  bool AsBool(const int64_t &row, const int64_t &column) const;
  double AsFloat(const int64_t &row, const int64_t &column) const;
  ctString AsString(const int64_t &row, const int64_t &column) const;

  // Set value will set or add the value
  void SetValue(const ctString &value, const int64_t &row, const int64_t &col);

  // Template version of SetValue() for convenience. An atToString() must exist for type 'T'
  template<typename T> void SetValue(const T &val, const int64_t &row, const int64_t &col);

  // Add value will fail if the cell already exists
  bool AddValue(const ctString &value, const int64_t &row, const int64_t &col);

  // Template version of AddValue() for convenience. An atToString() must exist for type 'T'
  template<typename T> bool AddValue(const T &val, const int64_t &row, const int64_t &col);

  ctString* TryGetValue(const int64_t &row, const int64_t &col);
  const ctString* TryGetValue(const int64_t &row, const int64_t &col) const;

  int64_t GetRowCount() const;
  int64_t GetColCount(const int64_t &row) const;

  friend ctString ctToString(const ctCSV &csv);

protected:
  ctVector<ctVector<ctString>> m_cells;
};

template<typename T> void ctCSV::SetValue(const T &val, const int64_t &row, const int64_t &col) { SetValue(ctToString(val), row, col); }
template<typename T> bool ctCSV::AddValue(const T &val, const int64_t &row, const int64_t &col) { return AddValue(ctToString(val), row, col); }

#endif // atCSV_h__
