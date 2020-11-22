
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "ctDateTime.h"
#include "ctScan.h"
#include <time.h>

ctDateTime::ctDateTime() { Set(time(nullptr)); }
ctDateTime::ctDateTime(const ctDateTime &o) { *this = o; }
ctDateTime::ctDateTime(ctDateTime &&o) { *this = std::move(o); }
ctDateTime& ctDateTime::operator=(ctDateTime &&o) { return *this = o;}
ctDateTime& ctDateTime::operator=(const ctDateTime &o)
{
  m_year = o.m_year;
  m_month = o.m_month;
  m_day = o.m_day;
  m_hour = o.m_hour;
  m_min = o.m_min;
  m_second = o.m_second;
  m_fmt = o.m_fmt;
  *m_pFullTime = *o.m_pFullTime;
  return *this;
}

ctDateTime::ctDateTime(const int64_t time) { Set(time); }
ctDateTime::ctDateTime(tm *pData) { Set(mktime(pData)); }
ctDateTime::~ctDateTime() { ctDelete(m_pFullTime); }

bool ctDateTime::Parse(const ctString &datetime, const atDateTimeFmt &fmt)
{
  int64_t datetimeVals[atDTC_Count] = { 0 };
  int64_t lastPos = 0;
  int64_t curTimePos = 0;
  while (true)
  {
    int64_t nextPos = datetime.find_first_of("- :/\\", lastPos);
    datetimeVals[curTimePos++] = ctScan::Int(datetime.c_str() + lastPos, nullptr, nextPos == CT_INVALID_INDEX ? -1 : nextPos - lastPos);
    if (nextPos == CT_INVALID_ID || curTimePos >= 6)
      break;
    lastPos = datetime.find_first_not("- :/\\", nextPos);
  }

  SetDay(datetimeVals[fmt.compIdx[atDTC_Day] + (fmt.dateFirst ? 0 : 3)]);
  SetMonth(datetimeVals[fmt.compIdx[atDTC_Month] + (fmt.dateFirst ? 0 : 3)]);
  SetYear(datetimeVals[fmt.compIdx[atDTC_Year] + (fmt.dateFirst ? 0 : 3)]);
  SetHour(datetimeVals[fmt.compIdx[atDTC_Hour] + (fmt.dateFirst ? 3 : 0)]);
  SetSecond(datetimeVals[fmt.compIdx[atDTC_Second] + (fmt.dateFirst ? 3 : 0)]);
  SetMin(datetimeVals[fmt.compIdx[atDTC_Minute] + (fmt.dateFirst ? 3 : 0)]);
  return true;
}

void ctDateTime::SetYear(int64_t year)
{
  m_pFullTime->dirty |= m_year != year;
  m_year = year;
}

ctDateTime::ctDateTime(const ctString &datetime, const atDateTimeFmt &fmt) { Parse(datetime, fmt); }

ctDateTime::ctDateTime(const int64_t &day, const int64_t &month, const int64_t &year, const int64_t &hour, const int64_t &min, const int64_t &sec)
{
  SetDay(day);
  SetMonth(month);
  SetYear(year);
  SetHour(hour);
  SetSecond(min);
  SetMin(sec);
}

void ctDateTime::SetMonth(int64_t mon)
{
  m_pFullTime->dirty |= m_month != mon;
  m_month = mon;
}

void ctDateTime::SetDay(int64_t day)
{
  m_pFullTime->dirty |= m_day != day;
  m_day = day;
}

void ctDateTime::SetHour(int64_t hr)
{
  m_pFullTime->dirty |= m_hour != hr;
  m_hour = hr;
}

void ctDateTime::SetMin(int64_t min)
{
  m_pFullTime->dirty |= m_min != min;
  m_min = min;
}

void ctDateTime::SetSecond(int64_t sec)
{
  m_pFullTime->dirty |= m_second != sec;
  m_second = sec;
}

const int64_t& ctDateTime::GetYear() const { return m_year; }
const int64_t& ctDateTime::GetMonth() const { return m_month; }
const int64_t& ctDateTime::GetDay() const { return m_day; }
const int64_t& ctDateTime::GetHour() const { return m_hour; }
const int64_t& ctDateTime::GetMin() const { return m_min; }
const int64_t& ctDateTime::GetSecond() const { return m_second; }

const int64_t& ctDateTime::to_time_t() const
{
  if (m_pFullTime->dirty)
  {
    time_t rawtime;
    time(&rawtime);
    tm *pTimeinfo = localtime(&rawtime);
    pTimeinfo->tm_hour = (int)GetHour();
    pTimeinfo->tm_min = (int)GetMin();
    pTimeinfo->tm_sec = (int)GetSecond();
    pTimeinfo->tm_mday = (int)GetDay();
    pTimeinfo->tm_mon = (int)GetMonth() - 1;
    pTimeinfo->tm_year = (int)GetYear() - 1900;
    m_pFullTime->val = (int64_t)mktime(pTimeinfo);
    m_pFullTime->dirty = false;
  }

  return m_pFullTime->val;
}

bool ctDateTime::operator>(const ctDateTime &rhs) const { return to_time_t() > rhs.to_time_t(); }
bool ctDateTime::operator<(const ctDateTime &rhs) const { return to_time_t() < rhs.to_time_t(); }
bool ctDateTime::operator==(const ctDateTime &rhs) const { return to_time_t() == rhs.to_time_t(); }
bool ctDateTime::operator>=(const ctDateTime &rhs) const { return !(*this < rhs); }
bool ctDateTime::operator<=(const ctDateTime &rhs) const { return !(*this > rhs); }
bool ctDateTime::operator!=(const ctDateTime &rhs) const { return !(*this == rhs); }

void ctDateTime::Set(const int64_t time)
{
  time_t myTime = time;
  tm *pData = localtime(&myTime);

  SetDay(pData->tm_mday);
  SetYear(pData->tm_year + 1900);
  SetMonth(pData->tm_mon + 1);

  SetMin(pData->tm_min);
  SetHour(pData->tm_hour);
  SetSecond(pData->tm_sec);
}

ctString ctToString(const ctDateTime &date)
{
  ctString dateTime;
  atDateTimeFmt fmt = date.m_fmt;

  bool isDate = fmt.dateFirst;
  int64_t lastIsDate = -1;
  int64_t componentsUsed = 0;
  int64_t lastIdx = 0;

  while (componentsUsed < 6)
  {
    int64_t minIdx = INT64_MAX;
    int64_t nextComp = -1;
    for (int64_t i = isDate ? 0 : 3; i < (isDate ? 3 : atDTC_Count); ++i)
      if (fmt.compIdx[i] < minIdx)
      {
        nextComp = i;
        minIdx = fmt.compIdx[i];
      }

    if (nextComp == -1)
      return dateTime;

    fmt.compIdx[nextComp] = INT64_MAX;

    switch (nextComp)
    {
    case atDTC_Day: dateTime += date.GetDay(); break;
    case atDTC_Month: dateTime += date.GetMonth(); break;
    case atDTC_Year: dateTime += date.GetYear(); break;
    case atDTC_Hour: dateTime += date.GetHour() % (fmt.base24Time ? 24 : 12); break;
    case atDTC_Minute: dateTime += date.GetMin(); break;
    case atDTC_Second: dateTime += date.GetSecond(); break;
    }

    ++componentsUsed;

    if (componentsUsed % 3 != 0)
    {
      dateTime += isDate ? fmt.dateSep : fmt.timeSep;
    }
    else
    {
      if (!fmt.base24Time && !isDate)
        dateTime += date.GetHour() >= 12 ? " PM": " AM";

      dateTime += fmt.dateTimeSep;
      isDate = !isDate;
    }
  }

  return dateTime;
}
