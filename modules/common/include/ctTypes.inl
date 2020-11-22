
template<typename T1, typename T2> bool __ctTypeCast(T1 *pDst, const T2 *pSrc, const int64_t &count)
{
  for (int64_t i = 0; i < count; ++i)
    pDst[i] = (T1)pSrc[i];
  return true;
}

template<typename T> bool __ctTypeCast(T *pDst, const T *pSrc, const int64_t &count)
{
  memcpy(pDst, pSrc, count * sizeof(T));
  return true;
}

template<typename T> bool __ctTypeCast(void *pDst, const T *pSrc, const ctType &dstType, const int64_t &count)
{
  switch (dstType)
  {
  case ctType_Float16: return false;
  case ctType_Float32: return __ctTypeCast((float*)pDst, pSrc, count);
  case ctType_Float64: return __ctTypeCast((double*)pDst, pSrc, count);
  case ctType_Int8: return __ctTypeCast((int8_t*)pDst, pSrc, count);
  case ctType_Int16: return __ctTypeCast((int16_t*)pDst, pSrc, count);
  case ctType_Int32: return __ctTypeCast((int32_t*)pDst, pSrc, count);
  case ctType_Int64: return __ctTypeCast((int64_t*)pDst, pSrc, count);
  case ctType_Uint8: return __ctTypeCast((uint8_t*)pDst, pSrc, count);
  case ctType_Uint16: return __ctTypeCast((uint16_t*)pDst, pSrc, count);
  case ctType_Uint32: return __ctTypeCast((uint32_t*)pDst, pSrc, count);
  case ctType_Uint64: return __ctTypeCast((uint64_t*)pDst, pSrc, count);
  }
  return false;
}
