#include "Statistics/ctPredictiveModel.h"

ctMatrix<double> ctPredictiveModel::PredictBatch(const ctMatrix<double> &input)
{
  // Convert to vectors
  ctVector<ctVector<double>> inVec;
  inVec.resize(input.m_rows);
  for (int64_t r = 0; r < input.m_rows; ++r)
    inVec[r].push_back(ctVector<double>(&input.at(r, 0), input.m_columns));

  // Make predictions
  ctVector<ctVector<double>> outVec = PredictBatch(inVec);
  if (outVec.size() == 0)
    return ctMatrix<double>();

  // Convert back to a matrix
  ctMatrix<double> outMat(outVec.front().size(), input.m_rows);
  for (int64_t r = 0; r < input.m_rows; ++r)
    for (int64_t c = 0; c < outVec[r].size(); ++c)
      outMat.at(r, c) = outVec[r][c];
  return outMat;
}

ctVector<ctVector<double>> ctPredictiveModel::PredictBatch(const ctVector<ctVector<double>> &input)
{
  ctVector<ctVector<double>> output;
  int64_t cols = -1;
  for (const ctVector<double> &in : input)
  {
    output.push_back(Predict(in));
    if (cols < 0)
      cols = output.back().size();
    if (output.back().size() != cols)
      return ctVector<ctVector<double>>();
  }
  return output;
}

bool ctPredictiveModel::TrainBatch(const ctMatrix<double> &input, const ctMatrix<double> &output)
{
  if (input.m_rows != output.m_rows)
    return false;

  ctVector<ctVector<double>> inVec;
  ctVector<ctVector<double>> outVec;
  inVec.resize(input.m_rows);
  outVec.resize(output.m_rows);
  for (int64_t r = 0; r < input.m_rows; ++r)
  {
    inVec[r].push_back(ctVector<double>(&input.at(r, 0), input.m_columns));
    outVec[r].push_back(ctVector<double>(&output.at(r, 0), output.m_columns));
  }

  return Train(inVec, outVec);
}
