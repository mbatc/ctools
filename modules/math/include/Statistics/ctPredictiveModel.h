#ifndef atPredictiveModel_h__
#define atPredictiveModel_h__

#include "../LinearAlgebra/ctMatrix.h"

// Predictive Model interface

class ctPredictiveModel
{
public:
  // Predict using a matrix of inputs and receive a matrix back
  // Each row is treated as an individual set of data
  virtual ctVector<double> Predict(const ctVector<double> &input) = 0;
  virtual bool Train(const ctVector<ctVector<double>> &input, const ctVector<ctVector<double>> &output) = 0;

  virtual int64_t InputCount() const = 0;
  virtual int64_t OutputCount() const = 0;

  ctMatrix<double> PredictBatch(const ctMatrix<double> &input);
  ctVector<ctVector<double>> PredictBatch(const ctVector<ctVector<double>> &input);
  bool TrainBatch(const ctMatrix<double> &input, const ctMatrix<double> &output);
};

#endif // atPredictiveModel_h__
