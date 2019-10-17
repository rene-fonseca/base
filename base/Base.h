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

#include <base/Trace.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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

#if 1
/**
  Returns the minimum value.
*/
template<class TYPE>
inline TYPE minimum(TYPE a, TYPE b) throw() {
  return (a <= b) ? a : b;
}

/**
  Returns the minimum value among the 3 values.
*/
template<class TYPE>
inline TYPE minimum(TYPE a, TYPE b, TYPE c) throw() {
  return minimum<TYPE>(minimum<TYPE>(a, b), c);
}

/**
  Returns the maximum value.
*/
template<class TYPE>
inline TYPE maximum(TYPE a, TYPE b) {
  return (a >= b) ? a : b;
}

/**
  Returns the maximum value among the 3 values.
*/
template<class TYPE>
inline TYPE maximum(TYPE a, TYPE b, TYPE c) {
  return maximum<TYPE>(maximum<TYPE>(a, b), c);
}
#else // TAG: need to avoid copy contruction
/**
  Returns the minimum value.
*/
template<class TYPE>
inline const TYPE& minimum(const TYPE& a, const TYPE& b) throw() {
  return (a <= b) ? a : b;
}

/**
  Returns the minimum value among the 3 values.
*/
template<class TYPE>
inline const TYPE& minimum(const TYPE& a, const TYPE& b, const TYPE& c) throw() {
  return minimum<TYPE>(minimum<TYPE>(a, b), c);
}

/**
  Returns the maximum value.
*/
template<class TYPE>
inline const TYPE& maximum(const TYPE& a, const TYPE& b) {
  return (a >= b) ? a : b;
}

/**
  Returns the maximum value among the 3 values.
*/
template<class TYPE>
inline const TYPE& maximum(const TYPE& a, const TYPE& b, const TYPE& c) {
  return maximum<TYPE>(maximum<TYPE>(a, b), c);
}
#endif

/**
  Returns the maximum value among the given values.
*/
#if 0
template<class TYPE>
inline TYPE maximum(initializer_list<TYPE> l) {
  ASSERT(!l.empty());
  auto src = l.begin();
  auto end = l.end();
  TYPE result = *src++;
  while (begin != end) {
    if (*src > result) {
      result = *src;
    }
    ++src;
  }
  return result;
}
#endif

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
template<class TYPE> inline TYPE absolute(const TYPE& value) throw() {
  return (value >= TYPE(0)) ? value : -value;
}

/**
  Raises the specified exception if the assertion is false.

  @param assertion The assertion.
  @param exception The exception.

  @ingroup debugging
  @see Assertion
*/
template<class EXCEPTION>
inline void bassert(bool assertion, const EXCEPTION& exception) throw(EXCEPTION) {
  if (!assertion) {
    throw exception;
  }
}

/**
  This class allows assertions to be specified outside a function body. This
  class may be used to check an assertion during application initialization.
  
  @short Assertion.
  @ingroup debugging
  @see assert
*/
class _COM_AZURE_DEV__BASE__API Assertion {
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



class _COM_AZURE_DEV__BASE__API Unspecified {
public:
  
  /** Constraint error codes. */
  enum {
    /** Unspecified error. */
    UNSPECIFIED
  };
};

/**
  Use this class to force an error at compile-time if the given expression
  evaluates to false.
  
  @short Compile-time constraint.
  @ingroup debugging
  
  @code
  class MyClass {
  public:
  
    enum {
      MY_ERROR
    };
    
    void myMethod() throw() {
      if (Constraint<sizeof(unsigned int) == sizeof(unsigned long)>::UNSPECIFIED);
      ...
      if (Constraint<sizeof(unsigned int) == sizeof(unsigned long), MyClass>::MY_ERROR);
      ...
    }
  };
  @endcode
*/
template<bool ASSERTION, class TYPE = Unspecified>
class Constraint {
};

template<class TYPE>
class Constraint<true, TYPE> : public TYPE {
};



template<class TYPE>
class Backend {
};



/** Destroys a complete object. */
template<class TYPE>
inline void deleteComplete(const volatile TYPE* value) {
  if (sizeof(TYPE) > 0) {}
  delete value;
}

/** Destroys a complete array. */
template<class TYPE>
inline void deleteCompleteArray(const volatile TYPE* value) {
  if (sizeof(TYPE) > 0) {}
  delete[] value;
}

/** Breakpoint. */
_COM_AZURE_DEV__BASE__API void breakpoint() noexcept;

// TAG: generate identifier automatically
_COM_AZURE_DEV__BASE__API void _COM_AZURE_DEV__BASE__VERSION_0() noexcept;

// Consume symbols to cause linker failure on mismatching shared/static builds
inline void _COM_AZURE_DEV__BASE__CONSUME_SYMBOL(void (*p)()) noexcept
{
  // do not call
  if (!p) {
    breakpoint();
  }
}

#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD) || defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY)
_COM_AZURE_DEV__BASE__API void _COM_AZURE_DEV__BASE__BUILD_SHARED() noexcept;
#else
_COM_AZURE_DEV__BASE__API void _COM_AZURE_DEV__BASE__BUILD_STATIC() noexcept;
#endif

inline void _COM_AZURE_DEV__BASE__BUILD_SHARED_STATIC_CONSUME() noexcept
{
#if !defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)
#  if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY)
  _COM_AZURE_DEV__BASE__CONSUME_SYMBOL(&_COM_AZURE_DEV__BASE__BUILD_SHARED);
#  else
  _COM_AZURE_DEV__BASE__CONSUME_SYMBOL(&_COM_AZURE_DEV__BASE__BUILD_STATIC);
#  endif
#endif
}

inline void _COM_AZURE_DEV__BASE__CHECK_VERSION() noexcept
{
  _COM_AZURE_DEV__BASE__CONSUME_SYMBOL(&_COM_AZURE_DEV__BASE__VERSION_0);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
