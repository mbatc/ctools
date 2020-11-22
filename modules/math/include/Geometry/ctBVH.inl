
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

template<typename T> ctBVH<T>::ctBVH(const ctVector<T> &primitives) { Construct(std::move(primitives)); }

template<typename T> void ctBVH<T>::Construct(const ctVector<T> &primitives)
{
  ctVector<ctBVHNode<T>> leaves(primitives.size());
  for (const T &prim : primitives)
  {
    ctBVHNode<T> leaf;
    leaf.bounds.GrowToContain(prim);
    leaf.primitive = prim;
    leaves.push_back(leaf);
    m_root.bounds.GrowToContain(leaf.bounds);
  }
  ConstructRecursive(&m_root, &leaves);
}

template<typename T> void ctBVH<T>::ConstructRecursive(ctBVHNode<T> *pRoot, ctVector<ctBVHNode<T>> *pLeaves)
{
  pRoot->children.resize(8);
  ctVec3D halfSize = pRoot->bounds.Dimensions() / 2;
  for (int64_t z = 0; z < 2; ++z)
    for (int64_t y = 0; y < 2; ++y)
      for (int64_t x = 0; x < 2; ++x)
      {
        pRoot->children[x + y * 2 + z * 4].bounds.m_min = pRoot->bounds.m_min + halfSize * ctVec3D{ x, y, z };
        pRoot->children[x + y * 2 + z * 4].bounds.m_max = pRoot->bounds.m_min + halfSize * ctVec3D{ x + 1, y + 1, z + 1 };
      }

  ctVector<ctVector<int64_t>> indices;

  for (int64_t c = 0; c < pRoot->children.size(); ++c)
  {
    int64_t nPrims = 0;
    int64_t lastPrim = 0;
    ctAABB<double> actualBounds;
    ctBVHNode<T> &child = pRoot->children[c];
    for (ctBVHNode<T> &prim : *pLeaves)
    {
      ctVector3<double> center = prim.primitive.Center();
      if (!child.bounds.Contains(center))
        continue;
      actualBounds.GrowToContain(prim.bounds);
      lastPrim = &prim - pLeaves->begin();
      ++nPrims;
    }

    if (nPrims == 0)
    {
      pRoot->children.erase(c);
      --c;
      continue;
    }
    else if (nPrims == 1)
    {
      child.isLeaf = true;
      child.primitive = (*pLeaves)[lastPrim].primitive;
      pLeaves->erase(lastPrim);
    }
    else
    {
      child.isLeaf = false;
    }

    if(!child.isLeaf)
      ConstructRecursive(&child, pLeaves);
    child.bounds = actualBounds;
  }
}

template<typename T> template<typename T2> bool ctBVH<T>::RayTrace(const ctRay<T2> &ray, ctMatrix4<T2> &modelMat, T2 *pTime) 
{ 
  ctMat4D invModel = modelMat.Inverse();
  ctVec3D startPos = invModel * ray.m_pos;
  ctVec3D endPos = invModel * (startPos + ray.m_dir);
  double invRayLen = (endPos - startPos).Length();
  double time = 0.0;
  bool result = ctIntersects(ctRay<T2>(startPos, endPos - startPos), *this, &time);
  time /= invRayLen;
  if (pTime)
    *pTime = time;
  return result; 
}