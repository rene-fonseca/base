/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FUNCTOR_H
#define _DK_SDU_MIP__BASE__FUNCTOR_H

#include "Base.h"



// Non-modifying operations



/**
  Returns true if two sequences are pairwise equal.
*/
template<class TYPE>
inline bool equal(const TYPE* left, const TYPE* right, unsigned int count) {
  while (count) {
    if (*left != *right) {
      return false;
    }
    ++left;
    ++right;
    --count;
  }
  return true;
}

/**
  Returns the index of the first mismatch between two sequences.

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
  Returns the number of matches of a predicate in a sequence.
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
  Invocates unary operation for each element.
*/
template<class TYPE, class UNOPR>
inline void forEach(const TYPE* left, unsigned int count, UNOPR& function) {
  while (count) {
    function(*left);
    ++left;
    --count;
  }
}

/**
  Invocates binary operation for each pair of elements.
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

/** Invocates binary operation for each element where the element is used as both arguments. */
template<class TYPE, class BINOPR>
inline void forEachDoBinary(const TYPE* left, unsigned int count, BINOPR& function) {
  while (count) {
    function(*left, *left);
    ++left;
    --count;
  }
}

/** Find first occurance of a value in a sequence. */
template<class TYPE>
inline const TYPE* find(const TYPE* left, unsigned int count, const TYPE& value) {
  while (count) {
    if (*left == value) { // do we have a match
      return left;
    }
    ++left;
    --count;
  }
  return NULL;
}

/**
  Finds the first match of a predicate in a sequence.

  @return Pointer to the value if found else NULL.
*/
template<class TYPE, class UNOPR>
inline const TYPE* find(const TYPE* left, unsigned int count, const UNOPR& predicate) {
  while (count) {
    if (predicate(*left)) { // do we have a match
      return left;
    }
    ++left;
    --count;
  }
  return NULL;
}



// Modifying operations



/** Apply an operation to every element in the sequence. */
template<class TYPE, class UNOPR>
inline void transform(TYPE* left, unsigned int count, const UNOPR& function) throw() {
  while (count) {
    *left = function(*left);
    ++left;
    --count;
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

/** Copies element by element from one sequence to another sequence. */
template<class TYPE>
inline void copy(TYPE* dest, const TYPE* src, unsigned int count) throw() {
  while (count) {
    *dest = *src;
    ++dest;
    ++src;
    --count;
  }
}

/** Swaps the elements of of two sequences. */
template<class TYPE>
inline void swap(TYPE* left, TYPE* right, unsigned int count) throw() {
  while (count) {
    swap(*left, *right);
    ++left;
    ++right;
    --count;
  }
}

/** Sets every element in the sequence to a specified value. */
template<class TYPE>
inline void fill(TYPE* dest, unsigned int count, const TYPE& value) throw() {
  while (count) {
    *dest = value;
    ++dest;
    --count;
  }
}



/** Base class of unary operations. */
template<class ARGUMENT, class RESULT>
class UnaryOperation {
public:
  typedef ARGUMENT ArgumentType;
  typedef RESULT ResultType;
};

/** Base class of binary operations. */
template<class FIRST, class SECOND, class RESULT>
class BinaryOperation {
public:
  typedef FIRST FirstType;
  typedef SECOND SecondType;
  typedef RESULT ResultType;
};



/** Binds a value to the first argument of a binary operation. */
template<class BINOPR>
class Binder2First : UnaryOperation<typename BINOPR::SecondType, typename BINOPR::ResultType> {
protected:
  const BINOPR& operation;
  typename BINOPR::FirstType first;
public:
  inline Binder2First(const BINOPR& opr, typename BINOPR::FirstType fst) throw() : operation(opr), first(fst) {};
  inline ResultType operator()(const typename BINOPR::SecondType& second) const throw() {return operation(first, second);};
};

/** Returns an unary operation from a binary operation using a value as the first argument. */
template<class BINOPR>
inline Binder2First<BINOPR> bind2First(const BINOPR& operation, const typename BINOPR::FirstType& value) {
  return Binder2First<BINOPR>(operation, value);
}

/** Binds a value to the second argument of a binary operation. */
template<class BINOPR>
class Binder2Second : UnaryOperation<typename BINOPR::FirstType, typename BINOPR::ResultType> {
protected:
  const BINOPR& operation;
  typename BINOPR::SecondType second;
public:
  inline Binder2Second(const BINOPR& opr, typename BINOPR::SecondType sec) throw() : operation(opr), second(sec) {};
  inline ResultType operator()(const typename BINOPR::FirstType& first) const throw() {return operation(first, second);};
};

/** Returns an unary operation from a binary operation using a value as the second argument. */
template<class BINOPR>
inline Binder2Second<BINOPR> bind2Second(const BINOPR& operation, const typename BINOPR::SecondType& value) {
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

#endif
