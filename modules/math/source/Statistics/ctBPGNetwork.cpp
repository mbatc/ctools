
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

#include "Statistics/ctBPGNetwork.h"

ctBPGNetwork::ctBPGNetwork(int64_t inputSize, int64_t outputSize, int64_t layerCount, int64_t layerSize)
{
  m_layerSize = layerSize;
  m_layers.resize(1 + ctMax(0, layerCount));
  m_layers[0].nNodes = inputSize;
  for (int64_t i = 1; i < m_layers.size() + 1; ++i)
  {
    int64_t nextLayerNodeCount = outputSize;
    if (i < m_layers.size())
    {
      m_layers[i].nNodes = layerSize;
      nextLayerNodeCount = layerSize;
    }

    m_layers[i - 1].weights = ctMatrix<double>(m_layers[i - 1].nNodes, nextLayerNodeCount);
    m_layers[i - 1].biases = ctMatrix<double>(1, nextLayerNodeCount);

    // Randomize values to begin with
    for (double &val : m_layers[i - 1].weights.m_data)
      val = ctClamp((double)(rand() % 500) / 500, 0.1, 1.0);
    for (double &val : m_layers[i - 1].biases.m_data)
      val = double(rand() % 1000ll - 500) / 500;
  }

  m_nOutputs = outputSize;
  m_nInputs = inputSize;
}

ctVector<double> ctBPGNetwork::Predict(const ctVector<double> &input) { return Predict(input, nullptr, nullptr).m_data; }

bool ctBPGNetwork::Train(const ctVector<ctVector<double>> &inputs, const ctVector<ctVector<double>> &outputs)
{
  ctMatrix<double> costGradient; // Cost gradient matrix
  ctVector<ctMatrix<double>> biasAdjustments;
  ctVector<ctMatrix<double>> weightAdjustments;
  // For each training set
  for (int64_t i = 0; i < inputs.size(); ++i)
  {
    const ctVector<double> &set = inputs[i];   // Input set
    const ctVector<double> &goal = outputs[i]; // Target values

    // Get the networks current prediction for the input set
    // We also need to store the activates at each layer to perform back propagation
    ctVector<ctMatrix<double>> activations;
    ctVector<ctMatrix<double>> rawActivations;
    Predict(set, &activations, &rawActivations);
    weightAdjustments.resize(activations.size() - 1);
    biasAdjustments.resize(activations.size() - 1);

    ctMatrix<double> prevLayerRawActivationCostInfluence;
    for (int64_t currentLayer = activations.size() - 1; currentLayer > 0; --currentLayer)
    {
      ctMatrix<double> &currentLayerRawActivations = rawActivations[currentLayer];
      ctMatrix<double> &prevLayerRawActivations = rawActivations[currentLayer - 1];
      ctMatrix<double> &currentLayerActivations = activations[currentLayer];
      ctMatrix<double> &prevLayerActivations = activations[currentLayer - 1];

      // Raw activation influences on the cost function for the previous layer
      ctMatrix<double> currentLayerRawWeightCostInfluence = prevLayerRawActivationCostInfluence;
      prevLayerRawActivationCostInfluence = ctMatrix<double>(1, prevLayerActivations.m_rows, 0);

      // Weight influences on the cost function for this layer
      ctMatrix<double> layerWeightCostInfluence(currentLayerActivations.m_rows, prevLayerActivations.m_rows);
      ctMatrix<double> layerLayerBiasCostInfluence(1, prevLayerRawActivations.m_rows);

      for (int64_t currentLayerNode = 0; currentLayerNode < currentLayerActivations.m_rows; ++currentLayerNode)
      {
        // double a = currentLayerActivations[currentLayerNode]; // Current layer activation value
        for (int64_t prevLayerNode = 0; prevLayerNode < prevLayerActivations.m_rows; ++prevLayerNode)
        {
          // double aPrev = prevLayerActivations[prevLayerNode]; // Previous layer activation value

          // Compute derivative of the current layers activation with respect to the rawActivation
          double sigmoidActivationInfluence = 1;
          if (m_activationFunc)
            sigmoidActivationInfluence = ctDerivative<double, double>(currentLayerRawActivations[currentLayerNode], m_activationFunc, 0.01);

          if (currentLayerRawWeightCostInfluence.m_rows == 0) // The current layer is the output layer
          {
            // Compute derivative of the Cost function with respect to the current layers activation
            double activationCostInfluence = 2 * (currentLayerActivations[currentLayerNode] - goal[currentLayerNode]);

            sigmoidActivationInfluence *= activationCostInfluence;
          }
          else
          {
            sigmoidActivationInfluence *= m_layers[currentLayer - 1].weights(prevLayerNode, currentLayerNode) * currentLayerRawWeightCostInfluence(currentLayerNode, 0);
          }

          // Compute derivative of the current layers activation with respect to the current layers weight
          double activationWeightInfluence = prevLayerActivations[prevLayerNode];

          // Compute derivative of the Cost function with respect to the previous layers raw activation
          double prevRawWeightCostInfluence = sigmoidActivationInfluence;

          // Compute derivative of the Cost function with respect to the weight
          double weightCostInfluence = prevRawWeightCostInfluence * activationWeightInfluence;

          // Add the weight influence to the weight cost influence
          layerWeightCostInfluence(prevLayerNode, currentLayerNode) += weightCostInfluence;

          layerLayerBiasCostInfluence(prevLayerNode, 0) += prevRawWeightCostInfluence;

          // Add the previous layers cost influence contributed by each node in this layer
          prevLayerRawActivationCostInfluence(prevLayerNode, 0) += prevRawWeightCostInfluence;
        }
      }

      if (weightAdjustments[currentLayer - 1].m_rows != layerWeightCostInfluence.m_rows)
        weightAdjustments[currentLayer - 1] = layerWeightCostInfluence;
      else
        weightAdjustments[currentLayer - 1] = weightAdjustments[currentLayer - 1] + layerWeightCostInfluence;

      if (biasAdjustments[currentLayer - 1].m_rows == 0)
        biasAdjustments[currentLayer - 1] = prevLayerRawActivationCostInfluence;
      else
        biasAdjustments[currentLayer - 1] = biasAdjustments[currentLayer - 1] + prevLayerRawActivationCostInfluence;
    }
  }

  // Adjust the training rate so that it depends on the number of nodes in the network
  double trainingAmount = m_trainingRate / (outputs.size() * m_nOutputs);
  for (int64_t i = 0; i < m_layers.size(); ++i)
    m_layers[i].weights = m_layers[i].weights - weightAdjustments[i].Mul(trainingAmount).Apply([](double val) { return ctClamp(val, -1.0, 1.0); });
  for (int64_t i = 0; i < m_layers.size(); ++i)
    m_layers[i].biases = m_layers[i].biases - biasAdjustments[i].Mul(trainingAmount);

  return true;
}

void ctBPGNetwork::SetActivationFunction(double(*func)(double)) { m_activationFunc = func; }

const ctMatrix<double>& ctBPGNetwork::GetLayerWeights(int64_t layer) const { return m_layers[layer].weights; }
const ctMatrix<double>& ctBPGNetwork::GetLayerBiases(int64_t layer) const { return m_layers[layer].biases; }
int64_t ctBPGNetwork::GetLayerSize(int64_t layer) const { return m_layers[layer].nNodes; }

int64_t ctBPGNetwork::LayerCount() const { return m_layers.size(); }
int64_t ctBPGNetwork::InputCount() const { return m_nInputs; }
int64_t ctBPGNetwork::OutputCount() const { return m_nOutputs; }

void ctBPGNetwork::SetTrainingRate(const double &rate) { m_trainingRate = rate; }
const double &ctBPGNetwork::GetTrainingRate() const { return m_trainingRate;  }

bool ctBPGNetwork::SetLayerWeights(int64_t layer, const ctMatrix<double> &weights)
{
  ctMatrix<double> &mat = m_layers[layer].weights;
  if (mat.m_columns != weights.m_columns || mat.m_rows != weights.m_rows)
    return false;
  mat = weights;
  return true;
}

bool ctBPGNetwork::SetLayerBiases(int64_t layer, const ctMatrix<double> &biases)
{
  ctMatrix<double> &mat = m_layers[layer].biases;
  if (mat.m_columns != biases.m_columns || mat.m_rows != biases.m_rows)
    return false;
  mat = biases;
  return true;
}

ctBPGNetwork::Layer::Layer()
  : weights(0, 0)
  , biases(0, 0)
{}

int64_t ctBPGNetwork::StreamWrite(ctWriteStream *pStream, const ctBPGNetwork *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
    ret += ctStreamWrite(pStream, &pData[i].m_layers, 1);
  return ret;
}

int64_t ctBPGNetwork::StreamRead(ctReadStream *pStream, ctBPGNetwork *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
    ret += ctStreamRead(pStream, &pData[i].m_layers, 1);
  return ret;
}

ctMatrix<double> ctBPGNetwork::Predict(const ctVector<double> &input, ctVector<ctMatrix<double>> *pActivations, ctVector<ctMatrix<double>> *pRawActivations)
{
  ctMatrix<double> activations(1, input.size());
  memcpy(activations.m_data.data(), input.data(), input.size() * sizeof(double));

  if (pRawActivations)
    pRawActivations->push_back(activations);
  if (pActivations)
    pActivations->push_back(activations);

  for (int64_t i = 0; i < m_layers.size(); ++i)
  {
    activations = (m_layers[i].weights * activations + m_layers[i].biases);
    if (pRawActivations)
      pRawActivations->push_back(activations);
    if (m_activationFunc)
      activations = activations.Apply(m_activationFunc);
    if (pActivations)
      pActivations->push_back(activations);
  }
  return activations;
}

int64_t ctStreamRead(ctReadStream *pStream, ctBPGNetwork::Layer *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += ctStreamRead(pStream, &pData[i].weights, 1);
    ret += ctStreamRead(pStream, &pData[i].biases, 1);
  }
  return ret;
}

int64_t ctStreamWrite(ctWriteStream *pStream, const ctBPGNetwork::Layer *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += ctStreamWrite(pStream, &pData[i].weights, 1);
    ret += ctStreamWrite(pStream, &pData[i].biases, 1);
  }
  return ret;
}

int64_t ctStreamRead(ctReadStream *pStream, ctBPGNetwork *pData, const int64_t count) { return ctBPGNetwork::StreamRead(pStream, pData, count); }
int64_t ctStreamWrite(ctWriteStream *pStream, const ctBPGNetwork *pData, const int64_t count) { return ctBPGNetwork::StreamWrite(pStream, pData, count); }
