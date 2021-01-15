
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

#include <algorithm>

template<typename T> ctMatrix<T> ctMatrix<T>::Identity(const int64_t &cols, const int64_t &rows)
{
  ctAssert(rows == cols, "rows and cols must be equal!");
  ctMatrix<T> ret(cols, rows);
  for (int64_t i = 0; i < rows; ++i)
    at(i, i) = (T)1;
  return ret;
}

template<typename T> ctMatrix<T> ctMatrix<T>::Transpose() const
{
  ctMatrix<T> ret(m_rows, m_columns);
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < m_columns; ++c)
      ret.at(c, r) = at(r, c);
  return ret;
}

template<typename T> T ctMatrix<T>::Determinate() const
{
  if (m_rows == 2)
    return m_data[0] * m_data[3] - m_data[1] * m_data[2];
  T ret = 0;
  for (int64_t c = 0; c < m_columns; ++c)
    ret += m_data[c] * LowOrderMatrix(c, 0, std::max(int64_t(2), m_rows - 1)).Determinate() * ((c % 2 == 0) ? 1 : -1);
  return ret;
}

template<typename T> ctMatrix<T> ctMatrix<T>::Cofactors() const
{
  ctAssert(m_rows == m_columns, "rows and col must be equal!");
  ctAssert(m_rows >= 2, "Matrix must be at least 2x2!");
  if (m_rows == 2)
    return ctMatrix<T>(2, 2, { m_data[0], -m_data[1], -m_data[2], m_data[3] });
  ctMatrix<T> ret(m_columns, m_rows);
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < m_columns; ++c)
      ret.at(r, c) = LowOrderMatrix(c, r, std::max(int64_t(2), m_rows - 1)).Determinate() * (((r + c) % 2 == 0) ? 1 : -1);
  return ret;
}

template<typename T> ctMatrix<T> ctMatrix<T>::LowOrderMatrix(const int64_t c, const int64_t r, const int64_t dim) const
{
  ctMatrix<T> ret(dim, dim);
  for (int64_t r2 = 0; r2 < dim; ++r2)
    for (int64_t c2 = 0; c2 < dim; ++c2)
      ret.at(r2, c2) = m_data[(c2 >= c ? c2 + 1 : c2) + (r2 >= r ? r2 + 1 : r2) * m_columns];
  return ret;
}

template<typename T> template<typename T2> ctMatrix<T> ctMatrix<T>::Mul(const ctMatrix<T2> &rhs) const
{
  ctMatrix<T> ret(rhs.m_columns, m_rows);
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < rhs.m_columns; ++c)
      for (int64_t i = 0; i < rhs.m_rows; ++i)
        ret.at(r, c) += at(r, i) * (T)rhs.at(i, c);
  return ret;
}

template<typename T> template<typename T2> ctMatrix<T> ctMatrix<T>::MulElementWise(const ctMatrix<T2> &rhs) const
{
  ctAssert(m_rows == rhs.m_rows && m_columns == rhs.m_columns, "Matrices must have the same dimensions to perform an element-wise multiply");
  ctMatrix<T> ret = *this;
  for (int64_t r = 0; r < m_rows; ++r)
    for (int64_t c = 0; c < m_columns; ++c)
      ret(r, c) *= (T)rhs(r, c);
  return ret;
}

template<typename T> template<typename T2> ctMatrix<T> ctMatrix<T>::Add(const ctMatrix<T2> &rhs) const
{
  ctMatrix<T> ret = *this;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] += (T)rhs.m_data[i];
  return ret;
}

template<typename T> template<typename T2> ctMatrix<T> ctMatrix<T>::Sub(const ctMatrix<T2> &rhs) const
{
  ctMatrix<T> ret = *this;
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] -= (T)rhs.m_data[i];
  return ret;
}

template<typename T>  ctMatrix<T> ctMatrix<T>::Mul(const T &rhs) const
{
  ctMatrix<T> ret(m_columns, m_rows);
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] = m_data[i] * rhs;
  return ret;
}

template<typename T> ctMatrix<T> ctMatrix<T>::Add(const T &rhs) const
{
  ctMatrix<T> ret(m_columns, m_rows);
  for (int64_t i = 0; i < m_columns * m_rows; ++i)
    ret[i] = m_data[i] + rhs;
  return ret;
}

template<typename T> inline ctMatrix<T> ctMatrix<T>::Apply(std::function<T(T)> func)
{
  ctMatrix<T> ret = *this;
  for (T &val : ret.m_data)
    val = func(val);
  return ret;
}

template<typename T> inline ctMatrix<T>::ctMatrix(int64_t _col, int64_t _row, const T &initialValue)
{
  m_columns = _col;
  m_rows = _row;
  m_data.reserve(_col * _row);
  m_data.resize(_col * _row, initialValue);
}

template<typename T> ctMatrix<T>::ctMatrix(int64_t _col, int64_t _row, const std::initializer_list<T> &list)
  : ctMatrix<T>(_col, _row)
{
  for (int64_t i = 0; i < (int64_t)list.size() && i < m_rows * m_columns; ++i)
    m_data[i] = (T)(*(list.begin() + i));
}

template<typename T> template <typename T2> const ctMatrix<T>& ctMatrix<T>::operator=(const ctMatrix<T2> &rhs)
{
  for (int64_t i = 0; i < m_rows * m_columns; ++i)
    m_data[i] = (T)rhs.m_data[i];
  return *this;
}

template<typename T> template <typename T2> ctMatrix<T>::ctMatrix(const ctMatrix<T2> &copy)
{
  for (int64_t i = 0; i < m_rows * m_columns; ++i)
    m_data[i] = (T)copy.m_data[i];
}

template<typename T> ctMatrix<T> ctMatrix<T>::Mul(const ctMatrix<T> &rhs) const { return Mul<T>(rhs); }
template<typename T> ctMatrix<T> ctMatrix<T>::MulElementWise(const ctMatrix<T> &rhs) const { return MulElementWise<T>(rhs); }

template<typename T> ctMatrix<T> ctMatrix<T>::operator*(const ctMatrix<T>& rhs) const { return Mul<T>(rhs); }
template<typename T> template <typename T2> ctMatrix<T> ctMatrix<T>::operator*(const ctMatrix<T2>& rhs) const { return Mul<T2>(rhs); }
template<typename T> template <typename T2> ctMatrix<T> ctMatrix<T>::operator/(const T2 &rhs) const { return Mul<T2>((T)1 / rhs); }
template<typename T> template <typename T2> ctMatrix<T> ctMatrix<T>::operator+(const ctMatrix<T2>& rhs) const { return Add(rhs); }
template<typename T> template <typename T2> ctMatrix<T> ctMatrix<T>::operator-(const ctMatrix<T2>& rhs) const { return Sub(rhs); }

template<typename T> ctMatrix<T> ctMatrix<T>::Inverse() const { return ((m_rows == 2 && m_columns == 2) ? ctMatrix<T>(2, 2, { m_data[3], -m_data[1], -m_data[2], m_data[0] }) : Cofactors().Transpose()).Mul((T)1 / Determinate()); }
template<typename T> const ctMatrix<T>& ctMatrix<T>::operator=(const ctMatrix<T> &rhs) { m_data = rhs.m_data; m_rows = rhs.m_rows; m_columns = rhs.m_columns; return *this; }

template<typename T> ctMatrix<T>::ctMatrix(ctMatrix<T> &&move) { m_data = move.m_data, m_columns = move.m_columns; m_rows = move.m_rows; }
template<typename T> ctMatrix<T>::ctMatrix(const ctMatrix<T> &copy) { m_data = copy.m_data; m_rows = copy.m_rows; m_columns = copy.m_columns; }

template<typename T> bool ctMatrix<T>::operator==(const ctMatrix<T> &rhs) const { return m_data == rhs.m_data; }
template<typename T> bool ctMatrix<T>::operator!=(const ctMatrix<T>& rhs) const { return !(*this == rhs); }
template<typename T> const T& ctMatrix<T>::operator[](const int64_t index) const { return m_data[index]; }
template<typename T> T& ctMatrix<T>::operator()(const int64_t row, const int64_t col) { return at(row, col); }
template<typename T> const T& ctMatrix<T>::operator()(const int64_t row, const int64_t col) const { return at(row, col); }
template<typename T> int64_t ctMatrix<T>::Rows() const { return m_rows; }
template<typename T> int64_t ctMatrix<T>::Columns() const { return m_columns; }

template<typename T> ctMatrix<double> ctMatrix<T>::ExtractMatrix(const ctVector<int64_t> &rows, const ctVector<int64_t> &columns) const { return ExtractRows(rows).ExtractColumns(columns); }

template<typename T> ctMatrix<double> ctMatrix<T>::ExtractRows(const ctVector<int64_t> &rows) const
{
  ctMatrix ret(Columns(), rows.size());
  for (int64_t c = 0; c < Columns(); ++c)
    for (int64_t r = 0; r < rows.size(); ++r)
      ret(r, c) = at(rows[r], c);
  return ret;
}

template<typename T> ctMatrix<double> ctMatrix<T>::ExtractColumns(const ctVector<int64_t> &columns) const
{
  ctMatrix ret(columns.size(), Rows());
  for (int64_t c = 0; c < columns.size(); ++c)
    for (int64_t r = 0; r < Rows(); ++r)
      ret(r, c) = at(r, columns[c]);
  return ret;
}

template<typename T> ctMatrix<T> ctMatrix<T>::Sub(const T &rhs) const { return Add(-rhs); }
template<typename T> T &ctMatrix<T>::at(const int64_t row, const int64_t col) { return m_data[col + row * m_columns]; }
template<typename T> const T &ctMatrix<T>::at(const int64_t row, const int64_t col) const { return m_data[col + row * m_columns]; }
template<typename T> T& ctMatrix<T>::operator[](const int64_t index) { return m_data[index]; }

template<typename T> inline int64_t ctStreamRead(ctReadStream *pStream, ctMatrix<T> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += ctStreamRead(pStream, &pData[i].m_rows, 1);
    ret += ctStreamRead(pStream, &pData[i].m_columns, 1);
    ret += ctStreamRead(pStream, &pData[i].m_data, 1);
  }
  return ret;
}

template<typename T> inline int64_t ctStreamWrite(ctWriteStream *pStream, const ctMatrix<T> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += ctStreamWrite(pStream, &pData[i].m_rows, 1);
    ret += ctStreamWrite(pStream, &pData[i].m_columns, 1);
    ret += ctStreamWrite(pStream, &pData[i].m_data, 1);
  }
  return ret;
}
