/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FUNCTOR_H
#define _DK_SDU_MIP__BASE__FUNCTOR_H

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
  state). Additionally, a relocateable object must not require to be initialized
  or destroyed to function properly. The relocateable property of an object is
  only intended to be used when really required due to performance
  considerations. If this function returns false objects have to be copy
  constructed at the new location and then destoyed at the old location. This
  function is primarily used by allocators to avoid these operations on their
  elements. Be very careful when adding your own specializations.
*/
template<class TYPE>
class Relocateable {
public:
  
  static const bool IS_RELOCATEABLE = primitives::Arithmetic<TYPE>::IS_ARITHMETIC;
};



// Non-modifying operations



/**
  Returns true if two sequences are pairwise equal (non-modifying operation).
*/
template<class TYPE>
inline bool equal(const TYPE* left, const TYPE* right, unsigned int count) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCMP)
  if (primitives::Arithmetic<TYPE>::IS_ARITHMETIC) {
    return isoc::memcmp(left, right, count * sizeof(TYPE)) == 0;
  } else {
#endif
    const TYPE* end = left + count;
    while (left < end) {
      if (*left != *right) {
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
  Returns the index of the first mismatch between two sequences (non-modifying operation).

  @return count if no mismatch was found.
*/
template<class TYPE>
inline unsigned int mismatch(const TYPE* left, const TYPE* right, unsigned int count) {
  unsigned int c = count;
  while (c) {
    if (*left != *right) {
      return count - c;
    }
    ++left;
    ++right;
    --c;
  }
  return count;
}

/**
  Compare the two sequences (non-modifying operation).
*/
template<class TYPE>
inline int compare(const TYPE* left, const TYPE* right, unsigned int count) {
  // I'm not using memcmp here 'cause this will give problems for big-endian architectures
  const TYPE* end = left + count;
  while (left < end) {
    int temp;
    if ((temp = compare(*left, *right)) != 0) {
      return temp;
    }
    ++left;
    ++right;
  }
  return 0; // equal
}

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCMP)
template<>
inline int compare<char>(const char* left, const char* right, unsigned int count) {
  return isoc::memcmp(left, right, count);
}

template<>
inline int compare<uint8>(const uint8* left, const uint8* right, unsigned int count) {
  return isoc::memcmp(left, right, count);
}
#endif

/**
  Returns the number of matches of a predicate in a sequence (non-modifying operation).
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
  Invocates the specified unary operation for each element (non-modifying operation).
*/
template<class TYPE, class UNOPR>
inline void forEach(const TYPE* element, unsigned int count, UNOPR& function) {
  const TYPE* end = element + count;
  --element;
  while (++element < end) {
    function(*element);
  }
}

/**
  Invocates the specified binary operation for each pair of elements (non-modifying operation).
*/
template<class TYPE, class BINOPR>
inline void forEach(const TYPE* left, const TYPE* right, unsigned int count, BINOPR& function) {
  while (count) {
    function(*left, *right);
    ++left;
    ++right;
    --count;
  }
}

/**
  Invocates the specified binary operation for each element where the element is used as both operands (non-modifying operation).
*/
template<class TYPE, class BINOPR>
inline void forEachDoBinary(const TYPE* left, unsigned int count, BINOPR& function) {
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
  const TYPE* end = element + count;
  while (element < end) {
    if (*element == value) { // do we have a match
      return element;
    }
    ++element;
  }
  return 0; // not found
}

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCHR)
template<>
inline const char* find(const char* element, unsigned int count, char value) throw() {
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
  Returns the index of the first occurance of a value in a sequence (non-modifying operation).

  @return -1 is not found.
*/
template<class TYPE>
inline int indexOf(const TYPE* element, unsigned int count, TYPE value) {
  const TYPE* current = element;
  const TYPE* end = element + count;
  while (current < end) {
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
inline void transform(TYPE* element, unsigned int count, const UNOPR& function) throw() {
  const TYPE* end = element + count;
  while (element < end) {
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
inline void transformByBinary(TYPE* restrict left, const TYPE* restrict right, unsigned int count, const BINOPR& function) throw() {
  while (count) {
    *left = function(*left, *right);
    ++left;
    ++right;
    --count;
  }
}

/** The sequences are expected not to overlap. */
template<class TYPE, class BINOPR>
inline void transformByBinary(TYPE* restrict result, const TYPE* restrict left, const TYPE* restrict right, unsigned int count, const BINOPR& function) throw() {
  while (count) {
    *result = function(*left, *right);
    ++result;
    ++left;
    ++right;
    --count;
  }
}

/**
  Copies element by element from one sequence to another sequence. This function
  expects the sequences to be non-overlapping. Relocateable objects are copied
  by copying their memory images directly (i.e. the copy constructor is
  circumvented). The size of the memory block must not exceed (2^(bits in
  unsigned int) - 1) bytes. The sequences should be aligned prior to invocation.

  @see move
*/
template<class TYPE>
inline void copy(TYPE* restrict dest, const TYPE* restrict src, unsigned int count) throw() {
  if (Relocateable<TYPE>::IS_RELOCATEABLE) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCPY)
    isoc::memcpy(dest, src, count * sizeof(TYPE));
#else
    // TAG: should I align the first long word
    uint64 bytesToCopy = static_cast<uint64>(count) * sizeof(TYPE);
    long* d = Cast::pointer<long*>(dest);
    const long* s = Cast::pointer<const long*>(src);
    {
      const TYPE* end = d + bytesToCopy/sizeof(long);
      while (d < end) {
        *d++ = *s++;
      }
    }
    {
      long* dc = Cast::pointer<char*>(d);
      const long* sc = Cast::pointer<char*>(s);
      const TYPE* end = dc + bytesToCopy % sizeof(long);
      while (dc < end) {
        *dc++ = *sc++;
      }
    }
#endif
  } else {
    const TYPE* end = dest + count;
    while (dest < end) {
      *dest++ = *src++;
    }
  }
}

/** Moves element by element from one sequence to another sequence (use this if the sequences may overlap). */
template<class TYPE>
inline void move(TYPE* dest, const TYPE* src, unsigned int count) throw() {
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
      bytesToMove%sizeof(long)
    );
#endif
  } else {
    if (dest < src) {
      const TYPE* end = dest + count;
      while (dest < end) {
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

/** Swaps the elements of of two sequences. The sequences are expected not to overlap. */
template<class TYPE>
inline void swap(TYPE* restrict left, TYPE* restrict right, unsigned int count) throw() {
  const TYPE* end = left + count;
  while (left < end) {
    swapper(*left++, *right++);
  }
}

/** Sets every element in the sequence to a specified value. */
template<class TYPE>
inline void fill(TYPE* dest, unsigned int count, TYPE value) throw() {
  const TYPE* end = dest + count;
  while (dest < end) {
    *dest++ = value;
  }
}

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMSET)
template<>
inline void fill<char>(char* dest, unsigned int count, char value) throw() {
  isoc::memset(dest, value, count);
}

template<>
inline void fill<uint8>(uint8* dest, unsigned int count, uint8 value) throw() {
  isoc::memset(dest, value, count);
}
#endif



/** Initializes an object to zero. This function should only be used for types with small memory footprints. */
template<class TYPE>
inline void clear(TYPE& value) throw() {
  long* p = Cast::pointer<long*>(&value);
  for (unsigned int i = 0; i < sizeof(TYPE)/sizeof(long); ++i) {
    *p++ = 0;
  }
  uint8* q = Cast::pointer<uint8*>(p);
  if (sizeof(TYPE)%sizeof(long) != 0) {
    for (unsigned int i = 0; i < sizeof(TYPE)%sizeof(long); ++i) {
      *q++ = 0;
    }
  }
}



/** Base class of unary operations. */
template<class ARGUMENT, class RESULT>
class UnaryOperation {
public:
  typedef ARGUMENT Argument;
  typedef RESULT Result;
};

/** Base class of binary operations. */
template<class FIRST, class SECOND, class RESULT>
class BinaryOperation {
public:
  typedef FIRST FirstArgument;
  typedef SECOND SecondArgument;
  typedef RESULT Result;
};



/** Binds a value to the first argument of a binary operation. */
template<class BINOPR>
class Binder2First : UnaryOperation<typename BINOPR::SecondArgument, typename BINOPR::Result> {
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

/** Returns an unary operation from a binary operation using a value as the first argument. */
template<class BINOPR>
inline Binder2First<BINOPR> bind2First(const BINOPR& operation, const typename BINOPR::FirstArgument& value) {
  return Binder2First<BINOPR>(operation, value);
}

/** Binds a value to the second argument of a binary operation. */
template<class BINOPR>
class Binder2Second : UnaryOperation<typename BINOPR::FirstArgument, typename BINOPR::Result> {
protected:
  
  typedef typename BINOPR::FirstArgument FirstArgument;
  typedef typename BINOPR::SecondArgument SecondArgument;
  typedef typename BINOPR::Result Result;
  
  const BINOPR& operation;
  SecondArgument second;
public:

  inline Binder2Second(const BINOPR& _operation, SecondArgument _second) throw() : operation(_operation), second(_second) {
  }

  inline Result operator()(const FirstArgument& first) const throw() {
    return operation(first, second);
  }
};

/** Returns an unary operation from a binary operation using a value as the second argument. */
template<class BINOPR>
inline Binder2Second<BINOPR> bind2Second(const BINOPR& operation, const typename BINOPR::SecondArgument& value) {
  return Binder2Second<BINOPR>(operation, value);
}



/** Arithmetic addition. */
template<class TYPE>
class Add : BinaryOperation<TYPE, TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {return left + right;}
};

/** Arithmetic subtraction. */
template<class TYPE>
class Subtract : BinaryOperation<TYPE, TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {return left - right;}
};

/** Arithmetic multiplication. */
template<class TYPE>
class Multiply : BinaryOperation<TYPE, TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {return left * right;}
};

/** Arithmetic division. */
template<class TYPE>
class Divide : BinaryOperation<TYPE, TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {return left/right;}
};

/** Arithmetic negation. */
template<class TYPE>
class Negate : UnaryOperation<TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left) const throw() {return -left;}
};

/** Returns the absolute value of the specified argument. */
template<class TYPE>
class Absolute : UnaryOperation<TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left) const {return absolute(left);}
};



/** Equality operator. */
template<class TYPE>
class Equal : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left == right;}
};

/** Inequality operator. */
template<class TYPE>
class NotEqual : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left != right;}
};

/** Greater than operator. */
template<class TYPE>
class Greater : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left > right;}
};

/** Less than operator. */
template<class TYPE>
class Less : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left < right;}
};

/** Greater than or equal operator. */
template<class TYPE>
class GreaterOrEqual : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left >= right;}
};

/** Less than or equal operator. */
template<class TYPE>
class LessOrEqual : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left <= right;}
};

/** Logical and. */
template<class TYPE>
class LogicalAnd : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left && right;}
};

/** Logical or. */
template<class TYPE>
class LogicalOr : public BinaryOperation<TYPE, TYPE, bool> {
public:
  inline bool operator()(const TYPE& left, const TYPE& right) const throw() {return left || right;}
};

/** Logical not. */
template<class TYPE>
class LogicalNot : public UnaryOperation<TYPE, bool> {
public:
  inline bool operator()(const TYPE& left) const throw() {return !left;}
};



/** Bitwise and. */
template<class TYPE>
class BitwiseAnd : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {return left & right;}
};

/** Bitwise or. */
template<class TYPE>
class BitwiseOr : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {return left | right;}
};

/** Bitwise exclusive or. */
template<class TYPE>
class BitwiseExclusiveOr : public BinaryOperation<TYPE, TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& left, const TYPE& right) const throw() {return left ^ right;}
};



/** Returns the value unchanged. */
template<class TYPE>
class Same : public UnaryOperation<TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& value) const throw() {return value;}
};

/** Sums elements in a sequence. */
template<class TYPE, class RESULT = TYPE>
class Sum : public UnaryOperation<TYPE, RESULT> {
protected:
  RESULT result;
public:
  inline Sum() throw() : result(0) {}
  inline void operator()(const TYPE& value) throw() {result += value;}
  inline RESULT getResult() const throw() {return result;}
};

/** Sums the square of elements in a sequence. */
template<class TYPE, class RESULT = TYPE>
class SquareSum : public UnaryOperation<TYPE, RESULT> {
protected:
  RESULT result;
public:
  inline SquareSum() throw() : result(0) {}
  inline void operator()(const TYPE& value) throw() {result += value * value;}
  inline RESULT getResult() const throw() {return result;}
};

/** Sums the product of elements in two sequences. */
template<class TYPE, class RESULT = TYPE>
class DotProduct : public BinaryOperation<TYPE, TYPE, RESULT> {
protected:
  RESULT result;
public:
  inline DotProduct() throw() : result(0) {}
  inline void operator()(const TYPE& left, const TYPE& right) throw() {result += left * right;}
  inline RESULT getResult() const throw() {return result;}
};

/** The minimum value in a sequence. */
template<class TYPE>
class Minimum : public UnaryOperation<TYPE, TYPE> {
protected:
  TYPE result;
public:
  inline Minimum(const TYPE& value = 0) throw() : result(value) {}
  inline void operator()(const TYPE& value) throw() {if (value < result) {result = value;}}
  inline TYPE getResult() const throw() {return result;}
};

/** The maximum value in a sequence. */
template<class TYPE>
class Maximum : public UnaryOperation<TYPE, TYPE> {
protected:
  TYPE result;
public:
  inline Maximum(const TYPE& value = 0) throw() : result(value) {}
  inline void operator()(const TYPE& value) throw() {if (value > result) {result = value;}}
  inline TYPE getResult() const throw() {return result;}
};



/**
  Class responsible for invocating member functions that takes no arguments.

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
  explicit inline InvokeMember(Member member) throw() : member(member) {}

  /**
    Initializes object from other object.
  */
  inline InvokeMember(const InvokeMember& copy) throw() : member(copy.member) {}

  /**
    Invocate member function.
  */
  inline RESULT operator()(TYPE* object) const {(object->*member)();}
};



/**
  Returns an InvocateMember object for the specified member function.

  @param member The member function to be invocated.
*/
template<class TYPE, class RESULT>
inline InvokeMember<TYPE, RESULT> invokeMember(RESULT (TYPE::*member)()) {
  return InvokeMember<TYPE, RESULT>(member);
}



/**
  Wrapper class used to invoke prefix and suffix pairs.

  <pre>
    class MyClass {
    public:

      MyClass();
      int myMethod(int);
    };

    class MyLock {
    public:

      MyLock();
      void lock();
      void unlock();
    };

    class MyPrefix {
    private:

      MyLock lock;
    public:

      MySuffix(MyLock l) : lock(l) {}
      inline operator()() {lock->lock();}
    };

    class MySuffix {
    private:

      MyLock lock;
    public:

      MySuffix(MyLock l) : lock(l) {}
      inline void operator()() {lock->unlock();}
    };

    void myFunction() {
      MyLock myLock;
      InvokeOutfix outfix(object, MyPrefix(myLock), MySuffix(myLock));
      int result = outfix->myMethod(1234);
    }
  </pre>

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

    Invoke(const Invoke& copy); // disable default copy constructor
    Invoke& operator=(const Invoke& eq); // disable default assignment
  public:

    inline Invoke(TYPE* o, PREFIX p, SUFFIX s) : object(o), prefix(p), suffix(s) {}

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
  inline InvokeOutfix(TYPE* object, PREFIX prefix, SUFFIX suffix) : invoke(object, prefix, suffix) {}

  /**
    Dereference.
  */
  inline Invoke operator->() {
    return invoke;
  }
};

/**
  Returns an InvokeOutfix object.
*/
template<class TYPE, class PREFIX, class SUFFIX>
inline InvokeOutfix<TYPE, PREFIX, SUFFIX> invokeOutfix(TYPE& object, PREFIX prefix, SUFFIX suffix) {
  return InvokeOutfix<TYPE, PREFIX, SUFFIX>(&object, prefix, suffix);
}

template<class TYPE, class PREFIX, class SUFFIX>
inline InvokeOutfix<TYPE, PREFIX, SUFFIX> invokeOutfix(TYPE* object, PREFIX prefix, SUFFIX suffix) {
  return InvokeOutfix<TYPE, PREFIX, SUFFIX>(object, prefix, suffix);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
