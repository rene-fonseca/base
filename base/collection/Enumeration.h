/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/collection/EndOfEnumeration.h>
#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// instantiate common types
#define _COM_AZURE_DEV__BASE__INSTANTIATE_CONTAINER_COMMON_TYPE(TYPE) \
  template class _COM_AZURE_DEV__BASE__API TYPE<bool>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<short>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned short>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<int>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned int>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<long>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned long>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<char>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned char>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<wchar>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<void*>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<float>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<double>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<long double>;
// TAG: add String and WideString also
//  template class _COM_AZURE_DEV__BASE__API TYPE<int64>; \
//  template class _COM_AZURE_DEV__BASE__API TYPE<uint64>; \

// temporary limited edition
#define _COM_AZURE_DEV__BASE__INSTANTIATE_CONTAINER_COMMON_TYPE_LIMITED(TYPE) \
  template class _COM_AZURE_DEV__BASE__API TYPE<bool>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<short>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned short>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<int>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned int>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<long>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned long>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<char>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<unsigned char>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<wchar>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<void*>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<float>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<double>; \
  template class _COM_AZURE_DEV__BASE__API TYPE<long double>;
//  template class _COM_AZURE_DEV__BASE__API TYPE<int64>; \
//  template class _COM_AZURE_DEV__BASE__API TYPE<uint64>; \

/**
  Defines the types of a modifying (has write access to the elements)
  enumerator.

  @short Descriptor for modifying enumerator.
  @ingroup collections
  @version 1.0
*/
template<class VALUE>
class EnumeratorTraits {
public:

  typedef EnumeratorTraits SelfEnumeratorTraits;

  typedef VALUE Value;
  typedef VALUE* Pointer;
  typedef MemoryDiff Distance;
};

/**
  Defines the types of a non-modifying enumerator (may only read the values of
  the elements).
  
  @short Descriptor for non-modifying enumerator.
  @ingroup collections
  @version 1.0
*/
template<class VALUE>
class ReadEnumeratorTraits {
public:

  typedef ReadEnumeratorTraits SelfEnumeratorTraits;

  typedef VALUE Value;
  typedef const VALUE* Pointer;
  typedef MemoryDiff Distance;
};

/**
  Enumeration interface used to traverse individual elements of a collection
  one by one. If possible, the enumeration interface should be used in
  preference to the iteration interface.

  @short Enumeration interface for modifying access
  @ingroup collections
  @version 1.0
*/

template<class TRAITS>
class Enumerator {
private:

  /**
    Used to ensure that the specified traits template argument is an enumerator.
  */
  typedef typename TRAITS::SelfEnumeratorTraits ValidTraits;
public:

  /** The type of the element. */
  typedef typename TRAITS::Value Value;
  /** The type of the difference between elements. */
  typedef typename TRAITS::Distance Distance;
  /** The type of a pointer to an element. */
  typedef typename TRAITS::Pointer Pointer;

  /**
    Returns true if the enumerator has more elements.
  */
  virtual bool hasNext() const noexcept = 0;

  /**
    Returns the next element of the enumerator.
  */
  virtual Pointer next() = 0;

  /**
    Destroys the enumerator.
  */
  virtual inline ~Enumerator()
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
