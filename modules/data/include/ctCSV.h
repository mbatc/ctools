#ifndef atCSV_h__
#define atCSV_h__

#include "ctStringValue.h"

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

  ctStringValue Get(const int64_t &row, const int64_t &column) const;
  ctType GetType(const int64_t &row, const int64_t &column) const;

  int64_t AsInt(const int64_t &row, const int64_t &column) const;
  bool AsBool(const int64_t &row, const int64_t &column) const;
  double AsFloat(const int64_t &row, const int64_t &column) const;
  ctString AsString(const int64_t &row, const int64_t &column) const;

  // Check if a cell contains no value
  bool IsEmpty(const int64_t &row, const int64_t &column) const;

  // Set value will set or add the value
  void Set(const ctStringValue &value, const int64_t &row, const int64_t &col);

  // Template version of SetValue() for convenience. An atToString() must exist for type 'T'
  template<typename T> void Set(const T &val, const int64_t &row, const int64_t &col);

  // Add value will fail if the cell already exists
  bool Add(const ctStringValue &value, const int64_t &row, const int64_t &col);

  // Template version of AddValue() for convenience. An atToString() must exist for type 'T'
  template<typename T> bool Add(const T &val, const int64_t &row, const int64_t &col);

  ctStringValue* TryGetValue(const int64_t &row, const int64_t &col);
  const ctStringValue* TryGetValue(const int64_t &row, const int64_t &col) const;

  int64_t GetRowCount() const;
  int64_t GetColCount(const int64_t &row) const;

  friend ctString ctToString(const ctCSV &csv);

protected:
  ctVector<ctVector<ctStringValue>> m_cells;
};

template<typename T> void ctCSV::Set(const T &val, const int64_t &row, const int64_t &col) { Set(ctStringValue(ctToString(val)), row, col); }
template<typename T> bool ctCSV::Add(const T &val, const int64_t &row, const int64_t &col) { return Add(ctToString(val), row, col); }

#endif // atCSV_h__
