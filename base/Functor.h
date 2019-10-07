/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Base.h>
#include <base/Primitives.h>
#include <base/Cast.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace isoc {

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCPY)
  extern "C" void* memcpy(void* restrict, const void* restrict, MemorySize);
#endif

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCMP)
  extern "C" int memcmp(const void*, const void*, MemorySize);
#endif

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMMOVE)
  extern "C" void* memmove(void*, const void*, MemorySize);
#endif

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCHR)
  extern "C" void* memchr(const void*, int, MemorySize);
#endif

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMSET)
  extern "C" void* memset(void*, int, MemorySize);
#endif
}; // end of namespace isoc

/**
  Specifies whether or not the type is relocateable (i.e. objects may be moved
  directly from one memory location to another without corrupting the object
  state). The relocateable property of an object is only intended to be used
  when really required due to performance considerations. If this function
  returns false objects have to be copy constructed at the new location and
  then destoyed at the old location. This function is primarily used by
  allocators to avoid these operations on their elements. Be very careful when
  adding your own specializations.

  @short Relocateable template class.
  @ingroup memory
*/
template<class TYPE>
class Relocateable {
public:
  
  static const bool IS_RELOCATEABLE = primitives::Primitive<TYPE>::IS_PRIMITIVE;
};

/**
  Specifies whether or not the object may be created and destroyed without
  invoking the constructor and destructor. This also implies that the object is
  relocatable. Be extremely careful when adding your own specializations.
  
  @short Uninitializeable template class.
  @ingroup memory
*/
template<class TYPE>
class Uninitializeable {
public:
  
  static const bool IS_UNINITIALIZEABLE = primitives::Primitive<TYPE>::IS_PRIMITIVE;
};



// Non-modifying operations



/**
  Returns true if two sequences are pairwise equal (non-modifying operation).
*/
template<class TYPE>
inline bool equal(
  const TYPE* left, const TYPE* right, unsigned int count) throw() {
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCMP)
  if (primitives::Arithmetic<TYPE>::IS_ARITHMETIC) {
    return isoc::memcmp(left, right, count * sizeof(TYPE)) == 0;
  } else {
#endif
    const TYPE* const end = left + count;
    while (left != end) {
      if (!(*left == *right)) {
        return false;
      }
      ++left;
      ++right;
    }
    return true;
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCMP)
  }
#endif
}

/**
  Returns the index of the first mismatch between two sequences (non-modifying
  operation).

  @param left The left sequence.
  @param right The right sequence.
  
  @return size if no mismatch was found.
*/
template<class TYPE>
inline unsigned int mismatch(
  const TYPE* left, const TYPE* right, unsigned int size) throw() {
  unsigned int temp = size;
  while (temp && (*left == *right)) {
    ++left;
    ++right;
    --temp;
  }
  return size - temp;
}

/**
  Compare the two sequences (non-modifying operation).
  
  @param left The left sequence.
  @param right The right sequence.
*/
template<class TYPE>
inline int compare(
  const TYPE* left, const TYPE* right, unsigned int size) throw() {
  const TYPE* const end = left + size;
  while (left != end) {
    int temp = compare(*left, *right);
    if (temp != 0) {
      return temp;
    }
    ++left;
    ++right;
  }
  return 0; // equal
}

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCMP)
template<>
inline int compare<char>(
  const char* left, const char* right, unsigned int count) throw() {
  return isoc::memcmp(left, right, count);
}

template<>
inline int compare<uint8>(
  const uint8* left, const uint8* right, unsigned int count) throw() {
  return isoc::memcmp(left, right, count);
}
#endif

/**
  Returns the number of matches of a predicate in a sequence (non-modifying
  operation).
*/
template<class TYPE, class UNOPR>
inline unsigned int count(const TYPE* left, unsigned int c, UNOPR& predicate) {
  unsigned int result = 0;
  while (c) {
    if (predicte(*left)) {
      ++result;
    }
    ++left;
    --c;
  }
  return result;
}

/**
  Invocates the specified unary operation for each element (non-modifying
  operation).
*/
template<class TYPE, class UNOPR>
inline void forEach(
  const TYPE* element, unsigned int count, UNOPR& function) {
  const TYPE* const end = element + count;
  --element;
  while (++element != end) {
    function(*element);
  }
}

/**
  Invocates the specified binary operation for each pair of elements
  (non-modifying operation).
*/
template<class TYPE, class BINOPR>
inline void forEach(
  const TYPE* left, const TYPE* right, unsigned int count, BINOPR& function) {
  while (count) {
    function(*left, *right);
    ++left;
    ++right;
    --count;
  }
}

/**
  Invocates the specified binary operation for each element where the element
  is used as both operands (non-modifying operation).
*/
template<class TYPE, class BINOPR>
inline void forEachDoBinary(
  const TYPE* left, unsigned int count, BINOPR& function) {
  while (count) {
    function(*left, *left);
    ++left;
    --count;
  }
}

/**
  Find the first occurance of a value in a sequence (non-modifying operation).

  @return A pointer to the value if it is present in the sequence otherwise 0.
*/
template<class TYPE>
inline const TYPE* find(const TYPE* element, unsigned int count, TYPE value) {
  const TYPE* const end = element + count;
  while (element != end) {
    if (*element == value) { // do we have a match
      return element;
    }
    ++element;
  }
  return 0; // not found
}

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCHR)
template<>
inline const char* find(const char* element, unsigned int count, char value) {
  return Cast::pointer<const char*>(isoc::memchr(element, value, count));
}
#endif

/**
  Finds the first match of a predicate in a sequence.

  @return Pointer to the value if found else 0.
*/
template<class TYPE, class UNOPR>
inline const TYPE* findPredicate(const TYPE* left, unsigned int count, const UNOPR& predicate) {
  while (count) {
    if (predicate(*left)) { // do we have a match
      return left;
    }
    ++left;
    --count;
  }
  return 0; // not found
}

/**
  Returns the index of the first occurance of a value in a sequence
  (non-modifying operation).

  @return -1 is not found.
*/
template<class TYPE>
inline int indexOf(const TYPE* element, unsigned int count, TYPE value) {
  const TYPE* current = element;
  const TYPE* const end = element + count;
  while (current != end) {
    if (*current == value) { // do we have a match
      return current - element;
    }
    ++current;
  }
  return -1; // not found
}



// Modifying operations



/** Apply an operation to every element in the sequence. */
template<class TYPE, class UNOPR>
inline void transform(TYPE* element, unsigned int count, const UNOPR& function) {
  const TYPE* const end = element + count;
  while (element != end) {
    *element = function(*element);
    ++element;
  }
}

/** The sequences are expected not to overlap. */
template<class TYPE, class UNOPR>
inline void transformByUnary(TYPE* restrict result, const TYPE* restrict left, unsigned int count, const UNOPR& function) throw() {
  while (count) {
    *result = function(*left);
    ++result;
    ++left;
    --count;
  }
}

/** The sequences are expected not to overlap. */
template<class TYPE, class BINOPR>
inline void transformByBinary(TYPE* restrict left, const TYPE* restrict right, unsigned int count, const BINOPR& function) {
  while (count) {
    *left = function(*left, *right);
    ++left;
    ++right;
    --count;
  }
}

/** The sequences are expected not to overlap. */
template<class TYPE, class BINOPR>
inline void transformByBinary(TYPE* restrict result, const TYPE* restrict left, const TYPE* restrict right, unsigned int count, const BINOPR& function) {
  while (count) {
    *result = function(*left, *right);
    ++result;
    ++left;
    ++right;
    --count;
  }
}

/**
  Copies element by element from one sequence to another sequence. This
  function expects the sequences to be non-overlapping. Relocateable objects
  are copied by copying their memory images directly (i.e. the copy constructor
  is circumvented). The size of the memory block must not exceed (2^(bits in
  unsigned int) - 1) bytes. The sequences should be aligned prior to
  invocation.
  
  @see move
*/
template<class TYPE>
inline void copy(TYPE* restrict dest, const TYPE* restrict src, unsigned int count) {
  if (Relocateable<TYPE>::IS_RELOCATEABLE) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCPY)
    isoc::memcpy(dest, src, count * sizeof(TYPE));
#else
    // TAG: should I align the first long word
    uint64 bytesToCopy = static_cast<uint64>(count) * sizeof(TYPE);
    long* d = Cast::pointer<long*>(dest);
    const long* s = Cast::pointer<const long*>(src);
    {
      const TYPE* const end = d + bytesToCopy/sizeof(long);
      while (d != end) {
        *d++ = *s++;
      }
    }
    {
      long* dc = Cast::pointer<char*>(d);
      const long* sc = Cast::pointer<char*>(s);
      const TYPE* const end = dc + bytesToCopy % sizeof(long);
      while (dc != end) {
        *dc++ = *sc++;
      }
    }
#endif
  } else {
    const TYPE* const end = dest + count;
    while (dest != end) {
      *dest++ = *src++;
    }
  }
}

/**
  Moves element by element from one sequence to another sequence (use this if
  the sequences may overlap).
*/
template<class TYPE>
inline void move(TYPE* dest, const TYPE* src, unsigned int count) {
  if (Relocateable<TYPE>::IS_RELOCATEABLE) {
    uint64 bytesToMove = static_cast<uint64>(count) * sizeof(TYPE);
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMMOVE)
    isoc::memmove(dest, src, bytesToMove);
#else
    // TAG: should I align the first long word
    long* d = Cast::pointer<long*>(dest);
    const long* s = Cast::pointer<long*>(src);
    move<long>(d, s, bytesToMove/sizeof(long));
    move<uint8>(
      Cast::pointer<uint8*>(d + bytesToMove/sizeof(long)),
      Cast::pointer<const uint8*>(s + bytesToMove/sizeof(long)),
      bytesToMove % sizeof(long)
    );
#endif
  } else {
    if (dest < src) {
      const TYPE* const end = dest + count;
      while (dest != end) {
        *dest++ = *src++;
      }
    } else {
      const TYPE* first = dest;
      dest += count;
      src += count;
      while (dest > first) {
        *--dest = *--src;
      }
    }
  }
}

/**
  Swaps the elements of of two sequences. The sequences are expected not to
  overlap.
*/
template<class TYPE>
inline void swap(TYPE* restrict left, TYPE* restrict right, unsigned int count) {
  const TYPE* const end = left + count;
  while (left != end) {
    swapper(*left++, *right++);
  }
}

/** Sets every element in the sequence to a specified value. */
template<class TYPE>
inline void fill(TYPE* dest, unsigned int count, TYPE value) {
  const TYPE* const end = dest + count;
  while (dest != end) {
    *dest++ = value;
  }
}

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMSET)
template<>
inline void fill<char>(char* dest, unsigned int count, char value) {
  isoc::memset(dest, value, count);
}

template<>
inline void fill<uint8>(uint8* dest, unsigned int count, uint8 value) {
  isoc::memset(dest, value, count);
}
#endif



/**
  Initializes an object to zero. This function should only be used for types
  with small memory footprints.
*/
template<class TYPE>
inline void clear(TYPE& value) throw() {
  // TAG: move to cpp?
  long* p = Cast::pointer<long*>(&value);
  const long* end = p + sizeof(value)/sizeof(long);
  while (p != end) {
    *p++ = 0;
  }
  uint8* q = Cast::pointer<uint8*>(p);
  MemorySize rest = sizeof(value) % sizeof(long);
  if (rest == 0) {
    return;
  }
  switch (rest) {
  case 7:
    *q++ = 0;
  case 6:
    *q++ = 0;
  case 5:
    *q++ = 0;
  case 4:
    *q++ = 0;
  case 3:
    *q++ = 0;
  case 2:
    *q++ = 0;
  case 1:
    *q++ = 0;
  case 0:
    break;
  default:
    for (MemorySize i = 0; i < rest; ++i) {
      *q++ = 0;
    }
  }
}



/**
  @short Base class of unary operations.
*/
template<class ARGUMENT, class RESULT>
class UnaryOperation {
public:
  typedef ARGUMENT Argument;
  typedef RESULT Result;
};

/**
  @short Base class of binary operations.
*/
template<class FIRST, class SECOND, class RESULT>
class BinaryOperation {
public:
  typedef FIRST FirstArgument;
  typedef SECOND SecondArgument;
  typedef RESULT Result;
};



/**
  Binds a value to the first argument of a binary operation.

  @short Binds to a value to the first operand of a binary operation.
*/
template<class BINOPR>
class Binder2First : public UnaryOperation<typename BINOPR::SecondArgument, typename BINOPR::Result> {
protected:

  typedef typename BINOPR::FirstArgument FirstArgument;
  typedef typename BINOPR::SecondArgument SecondArgument;
  typedef typename BINOPR::Result Result;
  
  const BINOPR& operation;
  FirstArgument first;
public:
  
  inline Binder2First(const BINOPR& _operation, FirstArgument _first) throw()
    : operation(_operation), first(_first) {
  }
  
  inline Result operator()(const SecondArgument& second) const throw() {
    return operation(first, second);
  }
};

/**
  Returns an unary operation from a binary operation using a value as the first
  argument.
*/
template<class BINOPR>
inline Binder2First<BINOPR> bind2First(
  const BINOPR& operation, const typename BINOPR::FirstArgument& value) {
  return Binder2First<BINOPR>(operation, value);
}

/**
  Binds a value to the second argument of a binary operation.
  
  @short Binds to a value to the second operand of a binary operation.
*/
template<class BINOPR>
class Binder2Second : public UnaryOperation<typename BINOPR::FirstArgument, typename BINOPR::Result> {
protected:
  
  typedef typename BINOPR::FirstArgument FirstArgument;
  typedef typename BINOPR::SecondArgument SecondArgument;
  typedef typename BINOPR::Result Result;
  
  const BINOPR& operation;
  SecondArgument second;
public:

  inline Binder2Second(const BINOPR& _operation, SecondArgument _second) throw()
    : operation(_operation), second(_second) {
  }

  inline Result operator()(const FirstArgument& first) const throw() {
    return operation(first, second);
  }
};

/**
  Returns an unary operation from a binary operation using a value as the second
  argument.
*/
template<class BINOPR>
inline Binder2Second<BINOPR> bind2Second(
  const BINOPR& operation, const typename BINOPR::SecondArgument& value) throw() {
  return Binder2Second<BINOPR>(operation, value);
}



/**
  @short Addition function object.
*/
template<class TYPE>
class Add : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left + right;
  }
};

/**
  @short Subtraction function object.
*/
template<class TYPE>
class Subtract : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left - right;
  }
};

/**
  @short Arithmetic multiplication function object.
*/
template<class TYPE>
class Multiply : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left * right;
  }
};

/**
  @short Arithmetic division function object.
*/
template<class TYPE>
class Divide : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left/right;
  }
};

/**
  @short Arithmetic negation function object.
*/
template<class TYPE>
class Negate : public UnaryOperation<TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left) const throw() {
    return -left;
  }
};

/**
  @short Absolute value function object.
*/
template<class TYPE>
class Absolute : public UnaryOperation<TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left) const throw() {
    return absolute(left);
  }
};



/**
  @short Equality function object.
*/
template<class TYPE>
class Equal : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return left == right;
  }
};

/**
  @short Inequality function object.
*/
template<class TYPE>
class NotEqual : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return !(left == right);
  }
};

/**
  @short Greater than function object.
*/
template<class TYPE>
class Greater : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return left > right;
  }
};

/**
  @short Less than function object.
*/
template<class TYPE>
class Less : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return left < right;
  }
};

/**
  @short Greater than or equal function object.
*/
template<class TYPE>
class GreaterOrEqual : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return left >= right;
  }
};

/**
  @short Less than or equal function object.
*/
template<class TYPE>
class LessOrEqual : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return left <= right;
  }
};

/**
  @short Logical and function object.
*/
template<class TYPE>
class LogicalAnd : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return left && right;
  }
};

/**
  @short Logical or function object.
*/
template<class TYPE>
class LogicalOr : public BinaryOperation<TYPE, TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {
    return left || right;
  }
};

/**
  @short Logical not function object.
*/
template<class TYPE>
class LogicalNot : public UnaryOperation<TYPE, bool> {
public:
  
  inline bool operator()(const TYPE& left) const throw() {
    return !left;
  }
};



/**
  @short Bitwise and function object.
*/
template<class TYPE>
class BitwiseAnd : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left & right;
  }
};

/**
  @short Bitwise or function object.
*/
template<class TYPE>
class BitwiseOr : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left | right;
  }
};

/**
  @short Bitwise exclusive or function object.
*/
template<class TYPE>
class BitwiseExclusiveOr : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {
    return left ^ right;
  }
};



/**
  @short Returns the value unchanged. */
template<class TYPE>
class Same : public UnaryOperation<TYPE, TYPE> {
public:
  
  inline TYPE operator()(const TYPE& value) const throw() {
    return value;
  }
};

/**
  This function object calculate the sum of a sequence of objects.
  
  @short Summation function object.
*/
template<class TYPE, class RESULT = TYPE>
class Sum : public UnaryOperation<TYPE, RESULT> {
protected:
  
  RESULT result = 0;
public:
  
  inline Sum() throw() {
  }
  
  inline void operator()(const TYPE& value) throw() {
    result += value;
  }
  
  inline RESULT getResult() const throw() {
    return result;
  }
};

/**
  This function object calculates the sum the absolute value of the objects of a
  sequence.

  @short Absolute summation function object.
*/
template<class TYPE, class RESULT = TYPE>
class AbsoluteSum : public UnaryOperation<TYPE, RESULT> {
protected:
  
  RESULT result = 0;
public:
  
  inline AbsoluteSum() throw() {
  }
  
  inline void operator()(const TYPE& value) throw() {
    result += absolute(value);
  }
  
  inline RESULT getResult() const throw() {
    return result;
  }
};

/**
  This function object calculates the sum of the square of the objects of a
  sequence.

  @short Square summation function object.
*/
template<class TYPE, class RESULT = TYPE>
class SquareSum : public UnaryOperation<TYPE, RESULT> {
protected:
  
  RESULT result = 0;
public:
  
  inline SquareSum() throw() {
  }

  inline void operator()(const TYPE& value) throw() {
    result += value * value;
  }
  
  inline RESULT getResult() const throw() {
    return result;
  }
};

/**
  This function object calculates the dot product of two sequences.
  
  @short Dot product function object.
*/
template<class TYPE, class RESULT = TYPE>
class DotProduct : public BinaryOperation<TYPE, TYPE, RESULT> {
protected:
  
  RESULT result = 0;
public:
  
  inline DotProduct() throw() {
  }
  
  inline void operator()(const TYPE& left, const TYPE& right) throw() {
    result += left * right;
  }
  
  inline RESULT getResult() const throw() {
    return result;
  }
};

/**
  This function object finds the minimum value of a sequence. The objects
  should be of an arithmetic type.
  
  @short Minimum value function object.
*/
template<class TYPE>
class Minimum : public UnaryOperation<TYPE, TYPE> {
protected:

  TYPE result = 0;
public:

  inline Minimum(const TYPE& value) throw() : result(value) {
  }
  
  inline void operator()(const TYPE& value) throw() {
    if (value < result) {
      result = value;
    }
  }

  inline TYPE getResult() const throw() {
    return result;
  }
};

/**
  This function object finds the minimum value of a sequence. The objects
  should be of an arithmetic type.
  
  @short Maximum value function object.
*/
template<class TYPE>
class Maximum : public UnaryOperation<TYPE, TYPE> {
protected:

  TYPE result = 0;
public:
  
  inline Maximum(const TYPE& value) throw()
    : result(value) {
  }
  
  inline void operator()(const TYPE& value) throw() {
    if (value > result) {
      result = value;
    }
  }
  
  inline TYPE getResult() const throw() {
    return result;
  }
};



/**
  Class responsible for invocating member functions that takes no arguments.

  @short Member invocation function object.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE, class RESULT>
class InvokeMember : public UnaryOperation<TYPE*, RESULT> {
private:

  /** The type of the member function. */
  typedef RESULT (TYPE::*Member)();
  /** The member function of the object. */
  Member member;

  /** Disable default assignment. */
  InvokeMember& operator=(const InvokeMember& eq) throw();
public:

  /**
    Initializes the object.

    @param member The member function to be invocated.
  */
  explicit inline InvokeMember(Member _member) throw()
    : member(_member) {
  }

  /**
    Initializes object from other object.
  */
  inline InvokeMember(const InvokeMember& copy) throw()
    : member(copy.member) {
  }

  /**
    Invocate member function.
  */
  inline RESULT operator()(TYPE* object) const {
    return (object->*member)();
  }
};

/**
  Class responsible for invocating const member functions that takes no
   arguments.

  @short Member invocation function object.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE, class RESULT>
class InvokeConstMember : public UnaryOperation<TYPE*, RESULT> {
private:

  /** The type of the member function. */
  typedef RESULT (TYPE::*Member)() const;
  /** The member function of the object. */
  Member member;

  /** Disable default assignment. */
  InvokeConstMember& operator=(const InvokeConstMember& eq) throw();
public:

  /**
    Initializes the object.

    @param member The member function to be invocated.
  */
  explicit inline InvokeConstMember(Member _member) throw()
    : member(_member) {
  }

  /**
    Initializes object from other object.
  */
  inline InvokeConstMember(const InvokeConstMember& copy) throw()
    : member(copy.member) {
  }

  /**
    Invocate member function.
  */
  inline RESULT operator()(TYPE* object) const {
    return (object->*member)();
  }
};



/**
  Returns an InvokeMember object for the specified member function.

  @param member The member function to be invocated.
*/
template<class TYPE, class RESULT>
inline InvokeMember<TYPE, RESULT> invokeMember(RESULT (TYPE::*member)()) {
  return InvokeMember<TYPE, RESULT>(member);
}

/**
  Returns an InvokeConstMember object for the specified member function.

  @param member The member function to be invocated.
*/
template<class TYPE, class RESULT>
inline InvokeConstMember<TYPE, RESULT> invokeMember(RESULT (TYPE::*member)() const) {
  return InvokeConstMember<TYPE, RESULT>(member);
}



/**
  Wrapper class used to invoke prefix and suffix pairs.

  @code
  class MyClass {
  public:
  
    MyClass() throw() {
    }
    
    int myMethod(int value) throw() {
    }
  };
  
  class MyLock {
  public:
  
    MyLock();
    
    void lock() throw(LockException);
    
    void unlock() throw(LockException);
  };
  
  class MyPrefix {
  private:
  
    MyLock lock;
  public:
  
    MySuffix(MyLock _lock) throw() : lock(_lock) {
    }
    
    inline operator()() throw(LockException) {
      lock->lock();
    }
  };
  
  class MySuffix {
  private:
  
    MyLock lock;
  public:
  
    MySuffix(MyLock _lock) throw() : lock(_lock) {
    }
    
    inline void operator()() throw(LockException) {
      lock->unlock();
    }
  };
  
  void myFunction() throw() {
    MyLock myLock;
    InvokeOutfix outfix(object, MyPrefix(myLock), MySuffix(myLock));
    int result = outfix->myMethod(1234);
  }
  @endcode
  
  @short Outfix invocation.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
template<class TYPE, class PREFIX, class SUFFIX>
class InvokeOutfix {
public:

  class Invoke {
  private:

    TYPE* object;
    PREFIX prefix;
    SUFFIX suffix;

    Invoke(const Invoke& copy) throw();
    Invoke& operator=(const Invoke& eq) throw();
  public:
    
    inline Invoke(TYPE* _object, PREFIX _prefix, SUFFIX _suffix)
      : object(_object), prefix(_prefix), suffix(_suffix) {
    }

    inline TYPE* operator->() {
      prefix();
      return object;
    }

    inline ~Invoke() {
      suffix();
    }
  };
private:

  Invoke invoke;
public:

  /**
    Initialize object with specified prefix and suffix.
  */
  inline InvokeOutfix(TYPE* object, PREFIX prefix, SUFFIX suffix) throw()
    : invoke(object, prefix, suffix) {
  }

  /**
    Dereference.
  */
  inline Invoke operator->() throw() {
    return invoke;
  }
};

/**
  Returns an InvokeOutfix object.
*/
template<class TYPE, class PREFIX, class SUFFIX>
inline InvokeOutfix<TYPE, PREFIX, SUFFIX> invokeOutfix(
  TYPE& object, PREFIX prefix, SUFFIX suffix) {
  return InvokeOutfix<TYPE, PREFIX, SUFFIX>(&object, prefix, suffix);
}

template<class TYPE, class PREFIX, class SUFFIX>
inline InvokeOutfix<TYPE, PREFIX, SUFFIX> invokeOutfix(
  TYPE* object, PREFIX prefix, SUFFIX suffix) {
  return InvokeOutfix<TYPE, PREFIX, SUFFIX>(object, prefix, suffix);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
