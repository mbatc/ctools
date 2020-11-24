
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

#ifndef _ctVector_h__
#define _ctVector_h__

#include "ctMemory.h"
#include "ctReadStream.h"
#include "ctWriteStream.h"
#include "ctIterator.h"
#include <vector>

template<typename T> class ctVector
{
  const double _grow_rate = 1.61803399; // PHI
public:
  typedef T ElementType;
  typedef T* vector_iterator;
  typedef const T* vector_const_iterator;

  ~ctVector();

  // Creates an empty ctVector
  ctVector();

  template<typename T2> ctVector(const ctVector<T2> &vec);

  // Creates an ctVector with an initial capacity of [reserve]
  ctVector(const int64_t _reserve);
  ctVector(const std::initializer_list<T> &list);
  ctVector(const T* pData, int64_t len);
  // Creates an ctVector with size [size] and copies [initial] into each element
  ctVector(const int64_t size, const T &initial);
  ctVector(const ctVector<T> &copy);
  ctVector(const std::vector<T> &copy);

  ctVector(ctVector<T> &&move);

  //***************************
  // Non-Const Member functions
  T& back();
  T& front();
  T& at(const int64_t index);

  // Retrieve a pointer to the internal memory block
  T* data();
  void set_data(T* pBuffer, const int64_t size, const int64_t capacity);
  T* take_data();

  void push_back(const ctVector<T> &item);
  void push_back(ctVector<T> &&item);
  void push_back(const T &item);
  void push_back(T &&item);
  void pop_back();
  void pop_front();
  void swap_pop_back(const int64_t index);
  void swap_pop_front(const int64_t index);

  void insert(const int64_t index, const T &item);
  void insert(const int64_t index, const ctVector<T> &items);
  void insert(const int64_t index, const std::vector<T> &items);
  void insert(const int64_t index, vector_const_iterator start, vector_const_iterator end);

  template<typename... Args> void emplace_back(Args&&... args);
  template<typename... Args> void emplace_back_array(const int64_t count, Args&&... args);
  template<typename... Args> void emplace_array(const int64_t index, const int64_t count, Args&&... args);
  template<typename... Args> void emplace(const int64_t index, Args&&... args);

  // Memory Management
  void resize(const int64_t size);
  void resize(const int64_t size, const T &initial);
  void reserve(const int64_t capacity);

  void shrink_to_fit();
  void clear();
  void make_empty();
  void erase(const int64_t index);
  void erase(const int64_t start, const int64_t end);
  void erase(vector_iterator start, vector_iterator end);
  void assign(const T &item, const int64_t count);
  void assign(vector_const_iterator start, vector_const_iterator end);

  // assign and attempt convert
  template<typename T1> void assign(const T1 *start, const T1 *end);

  void assign(const std::vector<T> &copy);
  void assign(const ctVector<T> &copy);
  void assign(const std::initializer_list<T> &list);

  // Access the element at [index]
  // Debug assert on invalid index
  T& operator[](const int64_t index);
  T& operator[](const int32_t index);
  T& operator[](const int16_t index);
  T& operator[](const int8_t index);

  //***************************
  // Const Member functions
  bool empty() const;
  const int64_t& size() const;
  const int64_t& capacity() const;

  const T& back() const;
  const T& front() const;
  const T& at(const int64_t index) const;

  // Retrieve a const-pointer to the internal memory block
  const T* data() const;

  // Const access to the element at [index]
  // Debug assert on invalid index

  const ctVector<T> &operator=(ctVector<T> &&rhs);
  const ctVector<T> &operator=(const ctVector<T> &rhs); 
  const ctVector<T> &operator+=(const ctVector<T> &rhs);
  const ctVector<T> &operator+(const ctVector<T> &rhs);
  const T& operator[](const int64_t index) const;
  const T& operator[](const int32_t index) const;
  const T& operator[](const int16_t index) const;
  const T& operator[](const int8_t index) const;

  void swap(ctVector<T> &with);
  
  void move_item(const int64_t from, const int64_t to);
  void move_to_index(const int64_t index, const int64_t to, const int64_t count = 1);
  void move_to_back(const int64_t index, const int64_t count = 1);
  void move_to_front(const int64_t index, const int64_t count = 1);

  //***************************
  // Iterator Member functions
  vector_iterator begin();
  vector_iterator end();
  vector_const_iterator begin() const;
  vector_const_iterator end() const;

  const bool operator==(const ctVector<T> &rhs) const;
  const bool operator!=(const ctVector<T> &rhs) const;

  operator std::vector<T>();
  template<typename T2> operator std::vector<T2>();

protected:

  bool shrink_by(const int64_t count);

  // Vector will only resize if [capacity] > [m_capacity]
  void grow_reserve(const int64_t capacity);
  bool try_grow(const int64_t minSize);

  // Return false if [m_capacity] < [size]
  // Sets [m_size] to [size] on success
  bool try_resize(const int64_t size, const T &initial);

  // Return false if [m_capacity] < [size]
  // Sets [m_size] to [size] on success
  bool try_resize(const int64_t size);

  // Returns false if [size] < [m_size]
  // Sets [m_pData] to new memory block on success
  bool try_realloc(const int64_t size);

  // Reallocate internal memory block
  // [size] must be >= [m_size]
  // Sets [m_capacity] to [size] on success
  void realloc(const int64_t size);
  
  //*****************
  // Member variables
  int64_t m_size = 0;
  int64_t m_capacity = 0;
  T *m_pData = nullptr;
};

template<typename T> ctTypeDesc ctGetTypeDesc(const ctVector<T> &vec)
{
  ctTypeDesc desc = ctGetTypeDesc<T>();
  desc.count *= vec.size();
  return desc;
}

template<typename T> int64_t ctStreamWrite(ctWriteStream *pStream, const ctVector<T> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (const ctVector<T> &vec : ctIterate(pData, count))
  {
    ret += ctStreamWrite(pStream, &vec.size(), 1);
    ret += ctStreamWrite(pStream, vec.data(), vec.size());
  }
  return ret;
}

template<typename T> int64_t ctStreamRead(ctReadStream *pStream, ctVector<T> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (ctVector<T> &vec : ctIterate(pData, count))
  {
    int64_t size = 0;
    ret += ctStreamRead(pStream, &size, 1);
    vec.resize(size);
    ret += ctStreamRead(pStream, vec.data(), size);
  }
  return ret;
}

#include "ctVector.inl"

#endif