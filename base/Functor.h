/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FUNCTOR_H
#define _DK_SDU_MIP__BASE__FUNCTOR_H

#include <base/Base.h>
#include <new>
#include <typeinfo>
#include <string.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// Type checking functions



#if defined(bool)
  #undef bool
#endif

/**
  Returns true if the type is void. Do NOT add specializations for this function.
*/
template<class TYPE> inline bool isVoid() {return false;}
template<> inline bool isVoid<void>() {return true;}

/**
  Returns true if the type is an integer type. Do NOT add specializations for this function.
*/
template<class TYPE> inline bool isInteger() {return false;}
template<> inline bool isInteger<bool>() {return true;}
template<> inline bool isInteger<char>() {return true;}
template<> inline bool isInteger<signed char>() {return true;}
template<> inline bool isInteger<unsigned char>() {return true;}
template<> inline bool isInteger<wchar_t>() {return true;}
template<> inline bool isInteger<int>() {return true;}
template<> inline bool isInteger<unsigned int>() {return true;}
template<> inline bool isInteger<long>() {return true;}
template<> inline bool isInteger<unsigned long>() {return true;}
template<> inline bool isInteger<long long>() {return true;}
template<> inline bool isInteger<unsigned long long>() {return true;}

/**
  Returns true if the type is a float, double, or long double. Do NOT add specializations for this function.
*/
template<class TYPE> inline bool isFloating() {return false;}
template<> inline bool isFloating<float>() {return true;}
template<> inline bool isFloating<double>() {return true;}
template<> inline bool isFloating<long double>() {return true;}

/**
  Returns true if the type is an arithmetic (integer or floating) type. Do NOT add specializations for this function.
*/
template<class TYPE> inline bool isArithmetic() {return isInteger<TYPE>() || isFloating<TYPE>();}

/**
  Returns true if the type is a primitive (built-in) type. Do NOT add specializations for this function.
  @see isRelocateable
*/
template<class TYPE> inline bool isPrimitive() {return isVoid<TYPE>() || isArithmetic<TYPE>();}

/**
  Returns true if objects of the specified type are relocateable (i.e. objects
  may be moved directly from one memory location to another). This function
  most only return true if the objects of the specified type are not required
  to be initialized and destroyed. If this function returns false the objects
  have to be copy constructed (new location) and then destoyed (old location).
  This function is primarily used by allocators to avoid operations on their
  elements. Be very careful when adding your own specializations.
*/
template<class TYPE> inline bool isRelocateable() {return isPrimitive<TYPE>();}



// Non-modifying operations



/**
  Returns true if two sequences are pairwise equal (non-modifying operation).
*/
template<class TYPE>
inline bool equal(const TYPE* left, const TYPE* right, unsigned int count) {
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCMP)
  if (isPrimitive<TYPE>()) {
    return memcmp(left, right, count * sizeof(TYPE)) == 0;
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
  return memcmp(left, right, count);
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
  return (const char*)memchr(element, value, count);
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

template<class TYPE, class UNOPR>
inline void transformByUnary(TYPE* result, const TYPE* left, unsigned int count, const UNOPR& function) throw() {
  while (count) {
    *result = function(*left);
    ++result;
    ++left;
    --count;
  }
}

template<class TYPE, class BINOPR>
inline void transformByBinary(TYPE* left, const TYPE* right, unsigned int count, const BINOPR& function) throw() {
  while (count) {
    *left = function(*left, *right);
    ++left;
    ++right;
    --count;
  }
}

template<class TYPE, class BINOPR>
inline void transformByBinary(TYPE* result, const TYPE* left, const TYPE* right, unsigned int count, const BINOPR& function) throw() {
  while (count) {
    *result = function(*left, *right);
    ++result;
    ++left;
    ++right;
    --count;
  }
}

/** Copies element by element from one sequence to another sequence (use this if the sequences aren't overlaping each other). */
template<class TYPE>
inline void copy(TYPE* dest, const TYPE* src, unsigned int count) throw() {
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCPY)
  if (isPrimitive<TYPE>()) {
    memcpy(dest, src, count * sizeof(TYPE));
  } else {
#endif
    const TYPE* end = dest + count;
    while (dest < end) {
      *dest = *src;
      ++dest;
      ++src;
    }
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMCPY)
  }
#endif
}

/** Moves element by element from one sequence to another sequence (use this if the sequences may overlap). */
template<class TYPE>
inline void move(TYPE* dest, const TYPE* src, unsigned int count) throw() {
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMMOVE)
  if (isPrimitive<TYPE>()) {
    memmove(dest, src, count * sizeof(TYPE));
  } else {
#endif
    if (dest < src) {
      const TYPE* end = dest + count;
      while (dest < end) {
        *dest = *src;
        ++dest;
        ++src;
      }
    } else {
      const TYPE* first = dest;
      dest += count;
      src += count;
      while (dest > first) {
        --dest;
        --src;
        *dest = *src;
      }
    }
#if defined(_DK_SDU_MIP__BASE__HAVE_MEMMOVE)
  }
#endif
}

/** Swaps the elements of of two sequences. */
template<class TYPE>
inline void swap(TYPE* left, TYPE* right, unsigned int count) throw() {
  const TYPE* end = left + count;
  while (left < end) {
    swapper(*left, *right);
    ++left;
    ++right;
  }
}

/** Sets every element in the sequence to a specified value. */
template<class TYPE>
inline void fill(TYPE* dest, unsigned int count, TYPE value) throw() {
  const TYPE* end = dest + count;
  while (dest < end) {
    *dest = value;
    ++dest;
  }
}

#if defined(_DK_SDU_MIP__BASE__HAVE_MEMSET)
template<>
inline void fill<char>(char* dest, unsigned int count, char value) throw() {
  memset(dest, value, count);
}
#endif



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
  const BINOPR& operation;
  typename BINOPR::FirstArgument first;
public:
  inline Binder2First(const BINOPR& opr, typename BINOPR::FirstArgument fst) throw() : operation(opr), first(fst) {};
  inline Result operator()(const typename BINOPR::SecondArgument& second) const throw() {return operation(first, second);};
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
  const BINOPR& operation;
  typename BINOPR::SecondArgument second;
public:
  inline Binder2Second(const BINOPR& opr, typename BINOPR::SecondArgument sec) throw() : operation(opr), second(sec) {};
  inline Result operator()(const typename BINOPR::FirstArgument& first) const throw() {return operation(first, second);};
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



/** Returns the element unchanged. */
/*template<class TYPE>
class Same : UnaryOperation<TYPE, TYPE> {
public:
  inline TYPE operator()(const TYPE& value) const throw() {return value;}
};*/



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

  @author René Møller Fonseca
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

  @author René Møller Fonseca
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
