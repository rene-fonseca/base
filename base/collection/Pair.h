/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__PAIR_H
#define _DK_SDU_MIP__BASE_COLLECTION__PAIR_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Binder of two values.

  @short Value pair binder
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
  inline Pair() : first(), second() {}

  /**
    Initializes a pair with the specified values.

    @param first The first value.
    @param second The second value.
  */
  inline Pair(const First& first, const Second& second);

  /**
    Initializes a pair from other pair.
  */
  inline Pair(const Pair& copy) : first(copy.first), second(copy.second) {}

  /**
    Assignment of pair by pair.
  */
  inline Pair& operator=(const Pair& eq) {
    if (&eq != this) { // protect against self assignment
      first = eq.first;
      second = eq.second;
    }
    return *this;
  }

  /**
    Returns the first value.
  */
  inline const First& getFirst() const {return first;}

  /**
    Returns the second value.
  */
  inline const Second& getSecond() const {return second;}

  /**
    Sets the first value.
  */
  inline void setFirst(const First& value) {first = value;}

  /**
    Sets the second value.
  */
  inline void setSecond(const Second& value) {second = value;}

  /**
    Returns -1, 0, or 1 if this pair is less than, equal to, or greater than the specified pair.
  */
  inline int compareTo(const Pair& eq) const {
    int result = compareTo(first, eq.first);
    return (result != 0) ? result : compareTo(second, eq.second);
  }

  /**
    Returns true if this pair is equal to the specified pair.
  */
  inline bool operator==(const Pair& eq) const {return (first == eq.first) && (second == eq.second);}

  /**
    Returns true if this pair is less than the specified pair.
  */
  inline bool operator<(const Pair& eq) const {return (first < eq.first) || (first == eq.first) && (second < eq.second);}

  /**
    Compares the specified pairs with each other.
  */
  friend int compare<>(const Pair& left, const Pair& right);
};

template<class FIRST, class SECOND>
inline Pair<FIRST, SECOND>::Pair(const First& _first, const Second& _second) : first(_first), second(_second) {
}

template<class FIRST, class SECOND>
inline int compare(const Pair<FIRST, SECOND>& left, const Pair<FIRST, SECOND>& right) {
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
inline Pair<FIRST, SECOND> makePair(const FIRST& first, const SECOND& second) {
  return Pair<FIRST, SECOND>(first, second);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
