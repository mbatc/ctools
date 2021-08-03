#include "ctPrint.h"

ctString ctPrint::Float(const double &val)
{
  char buffer[64];
  sprintf(buffer, "%f", val);
  return buffer;
}

ctString ctPrint::Int(const int64_t &val)
{
  char buffer[64];
  sprintf(buffer, "%ld", val);
  return buffer;
}

ctString ctPrint::Bool(const bool &val, const bool &verbose)
{
  return verbose ? (val ? "true" : "false") : (val ? "1" : "0");
}
