/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__BASE_H
#define _DK_SDU_MIP__BASE__BASE_H

#include <base/Trace.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This cast function is used to up cast pointers (i.e. you can explicitly
  specify the desired pointer type). This function only works if there exists
  implicit rules which convert the input type to the desired type.
*/
template<class TYPE>
inline TYPE up_cast(TYPE argument) throw() {return argument;}

/**
  Implicit casting function.
*/
template<class RESULT, class ARGUMENT>
inline RESULT implicit_cast(ARGUMENT argument) throw() {return argument;}

template<class TYPE> class ConstPointerHelper {
public:
  enum {IS_CONSTANT = false};
};

template<class TYPE> class ConstPointerHelper<const TYPE*> {
public:
  enum {IS_CONSTANT = true};
};

template<class TYPE> inline bool isConstPointer() throw() {
  return ConstPointerHelper<TYPE>::IS_CONSTANT;
}

template<class RESULT, class ARG> class PointerCastHelper {
public:
  // no casting function available
};

template<class RESULT, class ARG> class PointerCastHelper<RESULT*, ARG*> {
public:
  static inline RESULT* pointer_cast(ARG* value) throw() {return reinterpret_cast<RESULT*>(value);}
};

template<class RESULT, class ARG> class PointerCastHelper<const RESULT*, const ARG*> {
public:
  static inline const RESULT* pointer_cast(const ARG* value) throw() {return reinterpret_cast<const RESULT*>(value);}
};

/**
  This function casts any pointer type to any other pointer type (at compile
  time). You should definitely avoid this function when possible. However, it
  is not as dangerous as using reinterpret_cast directly. This function does
  not cast away the const qualifier.
*/
template<class RESULT, class ARG> inline RESULT pointer_cast(ARG value) throw() {
  return PointerCastHelper<RESULT, ARG>::pointer_cast(value);
}

/**
  Returns the minimum value.
*/
template<class TYPE>
inline TYPE minimum(TYPE a, TYPE b) throw() {return (a <= b) ? a : b;}

/**
  Returns the minimum value among the 3 values.
*/
template<class TYPE>
inline TYPE minimum(TYPE a, TYPE b, TYPE c) throw() {return minimum<TYPE>(minimum<TYPE>(a, b), c);}

/**
  Returns the maximum value.
*/
template<class TYPE>
inline TYPE maximum(TYPE a, TYPE b) throw() {return (a >= b) ? a : b;}

/**
  Returns the maximum value among the 3 values.
*/
template<class TYPE>
inline TYPE maximum(TYPE a, TYPE b, TYPE c) throw() {return maximum<TYPE>(maximum<TYPE>(a, b), c);}

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

/**
  Throws the specified exception if the assertion is false.

  @param assertion The assertion.
  @param exception The exception.

  @see Assertion
*/
template<class EXCEPTION>
inline void assert(bool assertion, EXCEPTION exception) throw(EXCEPTION) {
  if (!assertion) {
    throw exception;
  }
}

/**
  Assertion.

  @ingroup debugging
  @see assert
*/
class Assertion {
public:
  
  /**
    @param assertion The assertion.
    @param message The message.
  */
  inline Assertion(bool assertion, const char* message) throw() {
    if (!assertion) {
      Trace::message(message);
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
