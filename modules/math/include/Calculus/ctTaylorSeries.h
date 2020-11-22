#ifndef atTaylorSeries_h__
#define atTaylorSeries_h__

#include "ctVector.h"
#include <functional>

class ctTaylorSeries
{
public:
  ctTaylorSeries(double(*func)(double), const double &xPos = 0, const int64_t &depth = 3);

  void SetApproximationDepth(const int64_t &depth);
  void SetApproximationStart(const double &x);
  void SetFunction(double(*func)(double));

  double Get(const double &x) const;

protected:
  void CalculateTerms(const int64_t &depth);

  ctVector<double> m_coefficients;
  double m_approxStart = 0;
  double m_derivativeStep = 0.0001;
  double(*m_func)(double) = nullptr;
};

#endif // atTaylorSeries_h__
