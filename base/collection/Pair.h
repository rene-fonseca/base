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

#include <base/Base.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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
  Pair()
  {
  }

  /**
    Initializes a pair with the specified first value.

    @param first The first value.
    @param second The second value.
  */
  Pair(const First& _first)
    : first(_first)
  {
  }

  /**
    Initializes a pair with the specified values.

    @param first The first value.
    @param second The second value.
  */
  Pair(const First& _first, const Second& _second)
    : first(_first), second(_second)
  {
  }

  /**
    Initializes a pair with the specified values.

    @param first The first value.
    @param second The second value.
  */
  Pair(First&& _first, Second&& _second)
    : first(moveObject(_first)), second(moveObject(_second))
  {
  }

  /**
    Initializes a pair with the specified values.

    @param first The first value.
    @param second The second value.
  */
  Pair(const First& _first, Second&& _second)
    : first(_first), second(moveObject(_second))
  {
  }

  /**
    Initializes a pair with the specified values.

    @param first The first value.
    @param second The second value.
  */
  Pair(First&& _first, const Second& _second)
    : first(moveObject(_first)), second(_second)
  {
  }

  /**
    Initializes a pair from other pair.
  */
  Pair(const Pair& copy)
    : first(copy.first), second(copy.second)
  {
  }

  /**
    Initializes a pair from other pair.
  */
  Pair(Pair&& move)
    : first(moveObject(move.first)),
      second(moveObject(move.second))
  {
  }

  /**
    Assignment of pair by pair.
  */
  Pair& operator=(const Pair& assign)
  {
    if (&assign != this) { // protect against self assignment
      first = assign.first;
      second = assign.second;
    }
    return *this;
  }

  /**
    Assignment of pair by pair.
  */
  Pair& operator=(Pair&& assign)
  {
    if (&assign != this) { // protect against self assignment
      first = moveObject(assign.first);
      second = moveObject(assign.second);
    }
    return *this;
  }

  /**
    Returns the first value.
  */
  inline const First& getFirst() const noexcept
  {
    return first;
  }

  /**
    Returns the second value.
  */
  inline const Second& getSecond() const noexcept
  {
    return second;
  }

  /**
    Sets the first value.
  */
  void setFirst(const First& value)
  {
    first = value;
  }

  /**
    Sets the first value.
  */
  void setFirst(First&& value)
  {
    first = moveObject(value);
  }

  /**
    Sets the second value.
  */
  void setSecond(const Second& value)
  {
    second = value;
  }

  /**
    Sets the second value.
  */
  void setSecond(Second&& value)
  {
    second = moveObject(value);
  }

  /**
    Returns -1, 0, or 1 if this pair is less than, equal to, or greater than
    the specified pair.
  */
  inline int compareTo(const Pair& compare) const
  {
    int result = compareTo(first, compare.first);
    return (result != 0) ? result : compareTo(second, compare.second);
  }

  /**
    Returns true if this pair is equal to the specified pair.
  */
  inline bool operator==(const Pair& compare) const
  {
    return (first == compare.first) && (second == compare.second);
  }

  /**
    Returns true if this pair is equal to the specified pair.
  */
  inline bool operator!=(const Pair& compare) const
  {
    return !(operator==(compare));
  }

  /**
    Returns true if this pair is less than the specified pair.
  */
  inline bool operator<(const Pair& compare) const
  {
    return (first < compare.first) || ((first == compare.first) && (second < compare.second));
  }

  /**
    Compares the specified pairs with each other.
  */
  // friend int compare<>(const Pair& left, const Pair& right) noexcept;
};

template<class FIRST, class SECOND>
inline int compare(const Pair<FIRST, SECOND>& left, const Pair<FIRST, SECOND>& right)
{
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
inline Pair<FIRST, SECOND> makePair(const FIRST& first, const SECOND& second)
{
  return Pair<FIRST, SECOND>(first, second);
}

/**
  Makes a pair from the specified values.

  @param first The first value of the pair.
  @param second The second value of the pair.
*/

template<class FIRST, class SECOND>
inline Pair<FIRST, SECOND> makePair(FIRST&& first, SECOND&& second)
{
  return Pair<FIRST, SECOND>(moveObject(first), moveObject(second));
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
