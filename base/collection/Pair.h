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

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Binder of two values.

  @short Value pair binder
  @version 1.0
*/

template<class FIRST, class SECOND>
class Pair {
public:

  /** The type of the first value. */
  typedef FIRST First;
  /** The type of the second value. */
  typedef SECOND Second;
protected:

  /** The first value. */
  First first;
  /** The second value. */
  Second second;
public:

  /**
    Initializes a pair with default values.
  */
  inline Pair() throw() : first(), second() {
  }

  /**
    Initializes a pair with the specified values.

    @param first The first value.
    @param second The second value.
  */
  Pair(const First& first, const Second& second) throw();

  /**
    Initializes a pair from other pair.
  */
  inline Pair(const Pair& copy) throw()
    : first(copy.first), second(copy.second) {
  }

  /**
    Assignment of pair by pair.
  */
  inline Pair& operator=(const Pair& eq) throw() {
    if (&eq != this) { // protect against self assignment
      first = eq.first;
      second = eq.second;
    }
    return *this;
  }

  /**
    Returns the first value.
  */
  inline const First& getFirst() const throw() {
    return first;
  }

  /**
    Returns the second value.
  */
  inline const Second& getSecond() const throw() {
    return second;
  }

  /**
    Sets the first value.
  */
  inline void setFirst(const First& value) throw() {
    first = value;
  }

  /**
    Sets the second value.
  */
  inline void setSecond(const Second& value) throw() {
    second = value;
  }

  /**
    Returns -1, 0, or 1 if this pair is less than, equal to, or greater than
    the specified pair.
  */
  inline int compareTo(const Pair& eq) const throw() {
    int result = compareTo(first, eq.first);
    return (result != 0) ? result : compareTo(second, eq.second);
  }

  /**
    Returns true if this pair is equal to the specified pair.
  */
  inline bool operator==(const Pair& eq) const throw() {
    return (first == eq.first) && (second == eq.second);
  }

  /**
    Returns true if this pair is less than the specified pair.
  */
  inline bool operator<(const Pair& eq) const throw() {
    return (first < eq.first) || ((first == eq.first) && (second < eq.second));
  }

  /**
    Compares the specified pairs with each other.
  */
  // friend int compare<>(const Pair& left, const Pair& right) throw();
};

template<class FIRST, class SECOND>
inline Pair<FIRST, SECOND>::Pair(
  const First& _first, const Second& _second) throw()
  : first(_first), second(_second) {
}

template<class FIRST, class SECOND>
inline int compare(
  const Pair<FIRST, SECOND>& left, const Pair<FIRST, SECOND>& right) throw() {
  if (left.first < right.first) {
    return -1;
  } else if (left.first == right.first) {
    if (left.second < right.second) {
      return -1;
    } else if (left.second == right.second) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return 1;
  }
}

/**
  Makes a pair from the specified values.

  @param first The first value of the pair.
  @param second The second value of the pair.
*/

template<class FIRST, class SECOND>
inline Pair<FIRST, SECOND> makePair(
  const FIRST& first, const SECOND& second) throw() {
  return Pair<FIRST, SECOND>(first, second);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
