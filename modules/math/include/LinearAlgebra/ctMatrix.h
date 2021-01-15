
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

#ifndef atMatrixNxM_h__
#define atMatrixNxM_h__

#include <functional>
#include "ctVector.h"
#include "../ctMathHelpers.h"

template <typename T> class ctMatrix
{
public:
  ctMatrix(int64_t _col = 0, int64_t _row = 0, const T &initialValue = T(0));
  ctMatrix(int64_t _col, int64_t _row, const std::initializer_list<T> &list);
  ctMatrix(const ctMatrix<T> &copy);
  ctMatrix(ctMatrix<T> &&move);
  template <typename T2> ctMatrix(const ctMatrix<T2> &copy);

  static ctMatrix<T> Identity(const int64_t &cols, const int64_t &rows);
  ctMatrix<T> Transpose() const;
  ctMatrix<T> Cofactors() const;
  ctMatrix<T> Inverse() const;

  T Determinate() const;

  ctMatrix<T> Mul(const ctMatrix<T> &rhs) const;
  ctMatrix<T> MulElementWise(const ctMatrix<T> &rhs) const;
  ctMatrix<T> operator*(const ctMatrix<T> &rhs) const;

  ctMatrix<T> Mul(const T &rhs) const;
  ctMatrix<T> Sub(const T &rhs) const;
  ctMatrix<T> Add(const T &rhs) const;

  ctMatrix<T> Apply(std::function<T(T)> func);

  ctMatrix<T> LowOrderMatrix(const int64_t x, const int64_t y, const int64_t dim) const;

  template <typename T2> ctMatrix<T> Mul(const ctMatrix<T2> &rhs) const;
  template <typename T2> ctMatrix<T> MulElementWise(const ctMatrix<T2> &rhs) const;
  template <typename T2> ctMatrix<T> operator*(const ctMatrix<T2> &rhs) const;

  template <typename T2> ctMatrix<T> Add(const ctMatrix<T2> &rhs) const;
  template <typename T2> ctMatrix<T> Sub(const ctMatrix<T2> &rhs) const;

  template <typename T2> ctMatrix<T> operator/(const T2 &rhs) const;
  template <typename T2> ctMatrix<T> operator+(const ctMatrix<T2> &rhs) const;
  template <typename T2> ctMatrix<T> operator-(const ctMatrix<T2> &rhs) const;

  bool operator==(const ctMatrix<T> &rhs) const;
  bool operator!=(const ctMatrix<T> &rhs) const;
  const ctMatrix<T>& operator=(const ctMatrix<T> &copy);
  template <typename T2> const ctMatrix<T>& operator=(const ctMatrix<T2> &copy);

  T& at(const int64_t row, const int64_t col);
  const T& at(const int64_t row, const int64_t col) const;

  T& operator[](const int64_t index);
  const T& operator[](const int64_t index) const;

  T &operator()(const int64_t row, const int64_t col);
  const T &operator()(const int64_t row, const int64_t col) const;

  int64_t Rows() const;
  int64_t Columns() const;

  ctMatrix<double> ExtractMatrix(const ctVector<int64_t> &rows, const ctVector<int64_t> &columns) const;
  ctMatrix<double> ExtractRows(const ctVector<int64_t> &rows) const;
  ctMatrix<double> ExtractColumns(const ctVector<int64_t> &columns) const;

  ctVector<T> m_data;
  int64_t m_rows;
  int64_t m_columns;
};

template <typename T> int64_t ctStreamRead(ctReadStream *pStream, ctMatrix<T> *pData, const int64_t count);
template <typename T> int64_t ctStreamWrite(ctWriteStream *pStream, const ctMatrix<T> *pData, const int64_t count);

#include "ctMatrix.inl"

#endif // atMatrixNxM_h__
