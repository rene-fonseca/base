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

#include <base/Primitives.h>
#include <iterator>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Imports typedefs Value, Pointer, Reference, Distance, and Category from iterator traits into iterator. */
#define _COM_AZURE_DEV__BASE__IMPORT_ITERATOR_TRAITS(TRAITS) \
  typedef typename TRAITS::Value Value; \
  typedef typename TRAITS::Distance Distance; \
  typedef typename TRAITS::Reference Reference; \
  typedef typename TRAITS::Pointer Pointer; \
  typedef typename TRAITS::Category Category;

class InputIterator {};
class OutputIterator {};
class ForwardIterator : public InputIterator {};
class BidirectionalIterator : public ForwardIterator {};
class RandomAccessIterator : public BidirectionalIterator {};
class ContiguousIterator : public RandomAccessIterator {};

/**
  Defines the types of a modifying (has write access to the elements) iterator.

  @short Describes the types of a modifying iterator.
*/
template<class VALUE>
class IteratorTraits {
public:

  typedef IteratorTraits SelfIteratorTraits;
  typedef VALUE Value;
  typedef VALUE& Reference;
  typedef VALUE* Pointer;
  typedef MemoryDiff Distance;
  typedef RandomAccessIterator Category; // TAG: how to handle
};

/**
  Defines the types of a non-modifying iterator (may only read the values of the
  elements).

  @short Describes the type of a non-modifying iterator.
*/
template<class VALUE>
class ReadIteratorTraits {
public:

  typedef ReadIteratorTraits SelfIteratorTraits;
  typedef VALUE Value;
  typedef const VALUE& Reference;
  typedef const VALUE* Pointer;
  typedef MemoryDiff Distance;
  typedef RandomAccessIterator Category; // TAG: how to handle
};

/**
  The base class of all iterators.
  
  @short Iterator interface.
  @version 1.0
*/
template<class TRAITS>
class Iterator {
private:

  /**
    Used to ensure that the specified traits template argument is an iterator.
  */
  typedef typename TRAITS::SelfIteratorTraits ValidTraits;
public:

  /** The type of the element. */
  typedef typename TRAITS::Value Value;
  /** The type of the difference between elements. */
  typedef typename TRAITS::Distance Distance;
  /** The type of a reference to an element . */
  typedef typename TRAITS::Reference Reference;
  /** The type of a pointer to an element. */
  typedef typename TRAITS::Pointer Pointer;
  /** The iterator category. */
  typedef typename TRAITS::Category Category;
};

/** Convert base to std iterator category. */
template<class ITERATOR_CATEGORY>
class GetStdIteratorCategory {
public:

  typedef void iterator_category;
};

template<>
class GetStdIteratorCategory<InputIterator> {
public:

  typedef std::input_iterator_tag iterator_category;
};

template<>
class GetStdIteratorCategory<OutputIterator> {
public:

  typedef std::output_iterator_tag iterator_category;
};

template<>
class GetStdIteratorCategory<ForwardIterator> {
public:

  typedef std::forward_iterator_tag iterator_category;
};

template<>
class GetStdIteratorCategory<BidirectionalIterator> {
public:

  typedef std::bidirectional_iterator_tag iterator_category;
};

template<>
class GetStdIteratorCategory<RandomAccessIterator> {
public:

  typedef std::random_access_iterator_tag iterator_category;
};

#if 0
template<>
class GetStdIteratorCategory<ContiguousIterator> {
public:

  typedef std::contiguous_iterator_tag iterator_category;
};
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

/** Add std iterator traits for given base iterator. */
#define _COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(ITERATOR) \
namespace std {\
\
  template<class TYPE> \
  struct iterator_traits<typename ITERATOR<TYPE> > { \
    typedef typename ITERATOR<TYPE>::Value value_type; \
    typedef typename ITERATOR<TYPE>::Pointer pointer; \
    typedef typename ITERATOR<TYPE>::Reference reference; \
    typedef typename ITERATOR<TYPE>::Distance difference_type; \
    typedef typename base::GetStdIteratorCategory<typename ITERATOR<TYPE>::Category>::iterator_category iterator_category; \
  }; \
}

namespace std {

  template<class TRAITS>
  struct iterator_traits<typename base::IteratorTraits<TRAITS> > {
    typedef typename TRAITS::Value value_type;
    typedef typename TRAITS::Pointer pointer;
    typedef typename TRAITS::Reference reference;
    typedef typename TRAITS::Distance difference_type;
    typedef typename base::GetStdIteratorCategory<typename TRAITS::Category>::iterator_category iterator_category;
  };

  template<class TRAITS>
  struct iterator_traits<typename base::ReadIteratorTraits<TRAITS> > {
    typedef typename TRAITS::Value value_type;
    typedef typename TRAITS::Pointer pointer;
    typedef typename TRAITS::Reference reference;
    typedef typename TRAITS::Distance difference_type;
    typedef typename base::GetStdIteratorCategory<typename TRAITS::Category>::iterator_category iterator_category;
  };
}
