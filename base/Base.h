/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__BASE_H
#define _DK_SDU_MIP__BASE__BASE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/** This cast function is used to down cast pointers. */
template<class TYPE>
inline TYPE down_cast(TYPE argument) throw() {return argument;}

/**
  Returns the minimum value.
*/
template<class TYPE>
inline TYPE minimum(TYPE a, TYPE b) throw() {return (a <= b) ? a : b;}

/**
  Returns the maximum value.
*/
template<class TYPE>
inline TYPE maximum(TYPE a, TYPE b) throw() {return (a >= b) ? a : b;}

/**
  Returns -1, 0, and 1 if a is less than b, a is equal to b, and a is greater
  than b, respectively. The type must implement the less than and equal to
  operators.
*/
template<class TYPE>
inline int compare(const TYPE& a, const TYPE& b) throw() {
  if (a < b) {
    return -1;
  } else if (a == b) {
    return 0;
  } else {
    return 1;
  }
}

/**
  Returns the hash value of the specified object.
*/
template<class TYPE>
inline unsigned int hash(const TYPE& value) throw() {return 0;}

/**
  Swaps the values.
*/
template<class TYPE>
inline void swapper(TYPE& a, TYPE& b) throw() {
  TYPE temp = a;
  a = b;
  b = temp;
}

/**
  Returns the absolute value of the specified argument.
*/
template<class TYPE> inline TYPE absolute(const TYPE& value) {return (value >= TYPE(0)) ? value : -value;}
//template<class TYPE> inline TYPE absolute<double>(const TYPE& value) {return fabs(value);}

/**
  Throws the specified exception if the assertion is false.

  @param assertion The assertion.
  @param exception The exception.
*/
template<class EXCEPTION>
inline void assert(bool assertion, EXCEPTION exception) {
  if (!assertion) {
    throw exception;
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
