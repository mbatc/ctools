#ifndef atTimespan_h__
#define atTimespan_h__

#include <stdint.h>

#define atNANOSECOND  1
#define atMICROSECOND 1000
#define atMILLISECOND 1000000
#define atSECOND      1000000000
#define atMINUTE      60000000000
#define atHOUR        3600000000000
#define atDAY         86400000000000

template<int64_t UnitLength> class ctTimespanBase
{
public:
  // Construct from another timespan of the same units
  ctTimespanBase(const ctTimespanBase &span) : m_span(span.ToNano()) {}

  // Construct from another unit
  template<int64_t Unit2> ctTimespanBase(const ctTimespanBase<Unit2> &span) : m_span(span.ToNano()) {}

  // Create timespan from numeric values
  explicit ctTimespanBase(const double &value) : m_span(int64_t(value * UnitLength)) {}
  explicit ctTimespanBase(const int64_t &value) : m_span(value * UnitLength) {}

  // Convert a timespan to numeric values
  explicit operator int64_t() const { return m_span / UnitLength; }
  explicit operator double() const { return m_span / UnitLength; }

  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength>& operator =(const ctTimespanBase<Unit2> &rhs) { m_span = rhs.ToNano(); return *this; }

  // Timespan arithmetic
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength> operator +(const ctTimespanBase<Unit2> &rhs) const { return ctTimespanBase<1>(m_span + rhs.ToNano()); }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength> operator -(const ctTimespanBase<Unit2> &rhs) const { return ctTimespanBase<1>(m_span - rhs.ToNano()); }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength> operator /(const ctTimespanBase<Unit2> &rhs) const { return ctTimespanBase<1>(m_span / rhs.ToNano()); }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength> operator *(const ctTimespanBase<Unit2> &rhs) const { return ctTimespanBase<1>(m_span * rhs.ToNano()); }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength> operator %(const ctTimespanBase<Unit2> &rhs) const { return ctTimespanBase<1>(m_span % rhs.ToNano()); }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength>& operator +=(const ctTimespanBase<Unit2> &rhs) { *this = *this + rhs; return *this; }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength>& operator -=(const ctTimespanBase<Unit2> &rhs) { *this = *this - rhs; return *this; }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength>& operator *=(const ctTimespanBase<Unit2> &rhs) { *this = *this * rhs; return *this; }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength>& operator /=(const ctTimespanBase<Unit2> &rhs) { *this = *this / rhs; return *this; }
  template<int64_t Unit2 = UnitLength> ctTimespanBase<UnitLength>& operator %=(const ctTimespanBase<Unit2> &rhs) { *this = *this % rhs; return *this; }

  // Convert to another unit
  template<int64_t Unit2> operator ctTimespanBase<Unit2>() { return ctTimespanBase<Unit2>(m_span); }
  
  // Convert timespan to different units
  int64_t ToNano() const    { return m_span; }
  int64_t ToMicro() const   { return m_span / atMICROSECOND; }
  int64_t ToMilli() const   { return m_span / atMILLISECOND; }
  int64_t ToSeconds() const { return m_span / atSECOND; }
  int64_t ToMinutes() const { return m_span / atMINUTE; }
  int64_t ToHours() const   { return m_span / atHOUR; }
  int64_t ToDays() const    { return m_span / atDAY; }

  double ToNanof() const    { return (double)m_span; }
  double ToMicrof() const   { return (double)m_span / atMICROSECOND; }
  double ToMillif() const   { return (double)m_span / atMILLISECOND; }
  double ToSecondsf() const { return (double)m_span / atSECOND; }
  double ToMinutesf() const { return (double)m_span / atHOUR; }
  double ToHoursf() const   { return (double)m_span / atHOUR; }
  double ToDaysf() const    { return (double)m_span / atDAY; }

  // Get the length of the timespan in the Units specified by the template argument
  int64_t Get() const { return m_span / UnitLength; }
  double Getf() const { return (double)m_span / UnitLength; }

  ctTimespanBase &Set(const int64_t &length) { m_span = length * UnitLength; return *this; }
  ctTimespanBase &Set(const double &length) { m_span = int64_t(length * UnitLength); return *this; }

  // Comparison operator
  template<int64_t Unit2 = UnitLength> bool operator==(const ctTimespanBase<Unit2> &rhs) const { return m_span == rhs.ToNano(); }
  template<int64_t Unit2 = UnitLength> bool operator!=(const ctTimespanBase<Unit2> &rhs) const { return !(*this == rhs); }

protected:
  int64_t m_span;
};

// Common time units
typedef ctTimespanBase<atNANOSECOND>  ctNanoSeconds;
typedef ctTimespanBase<atMICROSECOND> ctMicroSeconds;
typedef ctTimespanBase<atMILLISECOND> ctMilliSeconds;
typedef ctTimespanBase<atSECOND>      ctSeconds;
typedef ctTimespanBase<atMINUTE>      ctMinutes;
typedef ctTimespanBase<atHOUR>        ctHours;
typedef ctTimespanBase<atDAY>         ctDays;
typedef ctNanoSeconds                 ctTimespan;

#endif // atTimespan_h__
