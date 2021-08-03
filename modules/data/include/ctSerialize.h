#ifndef atSerialize_h__
#define atSerialize_h__

#include "ctMath.h"
#include "ctObjectDescriptor.h"
#include "../../math/include/Statistics/ctBPGNetwork.h"

// Built-ins
void ctSerialize(ctObjectDescriptor *pSerialized, const bool &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const double &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const float &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const int8_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const int16_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const int32_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const uint8_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const int64_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const uint16_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const uint32_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const uint64_t &src);
void ctSerialize(ctObjectDescriptor *pSerialized, const char *src);
void ctSerialize(ctObjectDescriptor *pSerialized, const ctString &src);

void ctDeserialize(const ctObjectDescriptor &serialized, bool *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, double *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, float *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, int8_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, int16_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, int32_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, int64_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, uint8_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, uint16_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, uint32_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, uint64_t *pDst);
void ctDeserialize(const ctObjectDescriptor &serialized, ctString *pDrc);

// Math types
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector2<T> &src);
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector3<T> &src);
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector4<T> &src);
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctMatrix4<T> &src);
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctQuaternion<T> &src);

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector2<T> *pDst);
template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector3<T> *pDst);
template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector4<T> *pDst);
template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctMatrix4<T> *pDst);
template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctQuaternion<T> *pDst);

// Containers
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const T *pSrc, int64_t count);
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector<T> &src);
template<typename Key, typename Value> void ctSerialize(ctObjectDescriptor *pSerialized, const ctHashMap<Key, Value> &src);

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, T *pDst, int64_t count);
template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector<T> *pDst);
template<typename Key, typename Value> void ctDeserialize(const ctObjectDescriptor &serialized, ctHashMap<Key, Value> *pDst);

// BPG network serialize
void ctSerialize(ctObjectDescriptor *pSerialized, const ctBPGNetwork &src);
void ctDeserialize(const ctObjectDescriptor &serialized, ctBPGNetwork *pDst);

#include "ctSerialize.inl"
#endif // atSerialize_h__
