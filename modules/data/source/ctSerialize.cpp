#include "ctSerialize.h"

void ctSerialize(ctObjectDescriptor *pSerialized, const bool &src) { pSerialized->Set(src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const double &src) { pSerialized->Set(src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const float &src) { pSerialized->Set((double)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const int8_t &src) { pSerialized->Set((int64_t)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const int16_t &src) { pSerialized->Set((int64_t)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const int32_t &src) { pSerialized->Set((int64_t)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const uint8_t &src) { pSerialized->Set((int64_t)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const int64_t &src) { pSerialized->Set(src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const uint16_t &src) { pSerialized->Set((int64_t)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const uint32_t &src) { pSerialized->Set((int64_t)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const uint64_t &src) { pSerialized->Set((int64_t)src); }
void ctSerialize(ctObjectDescriptor *pSerialized, const char *src) { pSerialized->Set(ctString(src)); }
void ctSerialize(ctObjectDescriptor *pSerialized, const ctString &src) { pSerialized->Set(src); }

void ctDeserialize(const ctObjectDescriptor &serialized, bool *pDst) { *pDst = serialized.AsBool(); }
void ctDeserialize(const ctObjectDescriptor &serialized, double *pDst) { *pDst = serialized.AsFloat(); }
void ctDeserialize(const ctObjectDescriptor &serialized, float *pDst) { *pDst = (float)serialized.AsFloat(); }
void ctDeserialize(const ctObjectDescriptor &serialized, int8_t *pDst) { *pDst = (int8_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, int16_t *pDst) { *pDst = (int16_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, int32_t *pDst) { *pDst = (int32_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, int64_t *pDst) { *pDst = (int64_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, uint8_t *pDst) { *pDst = (uint8_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, uint16_t *pDst) { *pDst = (uint16_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, uint32_t *pDst) { *pDst = (uint32_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, uint64_t *pDst) { *pDst = (uint64_t)serialized.AsInt(); }
void ctDeserialize(const ctObjectDescriptor &serialized, ctString *pDst) { *pDst = serialized.AsString(); }

void ctSerialize(ctObjectDescriptor *pSerialized, const ctBPGNetwork &src)
{
  pSerialized->Add("learnRate").Serialize(src.GetTrainingRate());
  pSerialized->Add("nOutputs").Serialize(src.OutputCount());
  pSerialized->Add("nInputs").Serialize(src.InputCount());

  int64_t layerCount = src.LayerCount();

  if (layerCount > 1)
    pSerialized->Add("layerSize").Serialize(src.GetLayerSize(1));
  
  ctObjectDescriptor layers = pSerialized->Add("layers");
  for (int64_t layer = 0; layer < layerCount; ++layer)
  {
    const ctMatrix<double> &weights = src.GetLayerWeights(layer);
    const ctMatrix<double> &biases = src.GetLayerBiases(layer);

    layers.Add("bias").Serialize(biases);
    layers.Add("weight").Serialize(weights);
  }
}

void ctDeserialize(const ctObjectDescriptor &serialized, ctBPGNetwork *pDst)
{
  double trainingRate = 0;
  int64_t outputCount = 0;
  int64_t layerSize = 0;
  int64_t layerCount = 0;
  int64_t inputCount = 0;

  serialized["learnRate"].Deserialize(&trainingRate);
  serialized["nOutputs"].Deserialize(&outputCount);
  serialized["nInputs"].Deserialize(&inputCount);
  if (serialized.Get("layerSize").IsValue())
    serialized["layerSize"].Deserialize(&layerSize);

  ctObjectDescriptor layers = serialized["layers"];
  layerCount = layers.GetMemberCount();

  *pDst = ctBPGNetwork(inputCount, outputCount, layerCount - 1, layerSize);

  for (int64_t i = 0; i < layerCount; ++i)
  {
    ctMatrix<double> weights;
    ctMatrix<double> biases;
    layers["bias"].Deserialize(&biases);
    layers["weight"].Deserialize(&weights);
    pDst->SetLayerBiases(i, biases);
    pDst->SetLayerWeights(i, weights);
  }
}
