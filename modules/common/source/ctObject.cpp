
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

#include "ctObject.h"

ctObject::ctObject()
  : m_typeInfo(typeid(void))
  , m_destructFunc(nullptr)
{}

ctObject::ctObject(const ctObject &copy)
  : ctObject()
{
  Assign(copy);
}

ctObject::ctObject(ctObject &&move)
  : ctObject()
{
  Assign(std::move(move));
}

ctObject::~ctObject() { Destroy(); }

void ctObject::Assign(const ctObject &value)
{
  SetType(value);

  // Call type specific copy assign
  if (m_copyFunc)
    m_copyFunc(&m_data, value.m_data.data());

  // Copy members
  m_members = value.m_members;
}

void ctObject::Assign(ctObject &&value)
{
  SetType(value);

  // Call type specific move assign
  if (m_moveFunc)
    m_moveFunc(&m_data, value.m_data.data());

  // Move members
  m_members = std::move(value.m_members);
}

void ctObject::SetMember(const ctString &name, const ctObject &value)
{
  m_members[name] = value;
}

void ctObject::SetMember(const ctString &name, ctObject &&value)
{
  m_members[name] = std::move(value);
}

void ctObject::Destroy()
{
  if (m_destructFunc)
    m_destructFunc(&m_data);
  m_destructFunc = nullptr;
  m_copyFunc = nullptr;
  m_moveFunc = nullptr;
  m_typeInfo = typeid(void);
}

bool ctObject::HasMember(const ctString &name) const
{
  return m_members.Contains(name);
}

ctObject& ctObject::GetMember(const ctString &name)
{
  return m_members[name];
}

const ctObject& ctObject::GetMember(const ctString &name) const
{
  return m_members[name];
}

ctObject& ctObject::operator=(const ctObject &rhs)
{
  Assign(rhs);
  return *this;
}

ctObject& ctObject::operator=(ctObject &&rhs)
{
  Assign(std::move(rhs));
  return *this;
}

ctObject& ctObject::operator[](const ctString &name)
{
  return GetMember(name);
}

const ctObject& ctObject::operator[](const ctString &name) const
{
  return GetMember(name);
}

bool ctObject::Empty() const { return m_data.size() == 0 && m_members.Size() == 0 && m_typeInfo == typeid(void); }

ctString ctObject::Typename() const { return m_typeInfo.name(); }

ctVector<ctString> ctObject::GetMemberNames() const { return m_members.GetKeys(); }

int64_t ctStreamRead(ctReadStream *pStream, ctObject *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
    if (!pData[i].Is<void>())
    {
      size += pData[i].m_readFunc(pStream, &pData->m_data);
      size += ctStreamRead(pStream, &pData->m_members, 1);
    }

  return size;
}

int64_t ctStreamWrite(ctWriteStream *pStream, ctObject *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
    if (!pData[i].Is<void>())
    {
      size += pData[i].m_writeFunc(pStream, &pData->m_data);
      size += ctStreamWrite(pStream, &pData->m_members, 1);
    }

  return size;
}
