
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

#ifndef atBPGNetwork_h__
#define atBPGNetwork_h__

#include "ctPredictiveModel.h"
#include "../LinearAlgebra/ctMatrix.h"

class ctBPGNetwork : public ctPredictiveModel
{
public:
  struct Layer
  {
    Layer();
    int64_t nNodes;
    ctMatrix<double> weights;
    ctMatrix<double> biases;
  };

  ctBPGNetwork(int64_t inputSize, int64_t outputSize, int64_t layerCount, int64_t layerSize = 16);

  virtual ctVector<double> Predict(const ctVector<double> &input);

  // Train on a specific input/output
  virtual bool Train(const ctVector<ctVector<double>> &input, const ctVector<ctVector<double>> &output);

  void SetActivationFunction(double(*func)(double));

  // To modify a layers weights/bias use the Get functions to retrieve the current
  // values, modify the returned matrices and then use the Set functions to set
  // the networks values.
  //
  // The Set functions will fail if the matrix passed is the incorrect size.
  const ctMatrix<double>& GetLayerWeights(int64_t layer) const;
  const ctMatrix<double>& GetLayerBiases(int64_t layer) const;
  int64_t GetLayerSize(int64_t layer) const;

  void SetTrainingRate(const double &rate);
  const double& GetTrainingRate() const;

  bool SetLayerWeights(int64_t layer, const ctMatrix<double> &weights);
  bool SetLayerBiases(int64_t layer, const ctMatrix<double> &biases);

  int64_t LayerCount() const;
  int64_t InputCount() const override;
  int64_t OutputCount() const override;

  static int64_t StreamWrite(ctWriteStream *pStream, const ctBPGNetwork *pData, const int64_t count);
  static int64_t StreamRead(ctReadStream *pStream, ctBPGNetwork *pData, const int64_t count);

protected:
  ctMatrix<double> Predict(const ctVector<double> &input, ctVector<ctMatrix<double>> *pActivations, ctVector<ctMatrix<double>> *pRawActivations);

  double m_trainingRate = 0.1;
  int64_t m_nInputs = 0;
  int64_t m_nOutputs = 0;
  int64_t m_layerSize = 0;
  ctVector<Layer> m_layers;
  double(*m_activationFunc)(double); // = atSigmoid<double>;
};

int64_t ctStreamRead(ctReadStream *pStream, ctBPGNetwork::Layer *pData, const int64_t count);
int64_t ctStreamWrite(ctWriteStream *pStream, const ctBPGNetwork::Layer *pData, const int64_t count);
int64_t ctStreamRead(ctReadStream *pStream, ctBPGNetwork *pData, const int64_t count);
int64_t ctStreamWrite(ctWriteStream *pStream, const ctBPGNetwork *pData, const int64_t count);

#endif // atBPGNetwork_h__
