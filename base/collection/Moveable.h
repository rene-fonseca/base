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

#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Class for getting stats for container manipulations and similar. Not MT-safe. */
class _COM_AZURE_DEV__BASE__API Moveable {
public:

  class _COM_AZURE_DEV__BASE__API Stats : public ReferenceCountedObject {
  public:

    MemorySize constructs = 0;
    MemorySize destructs = 0;
    MemorySize copies = 0;
    MemorySize moves = 0;

    void reset()
    {
      copies = 0;
      moves = 0;
      constructs = 0;
      destructs = 0;
    }

    bool operator==(const Stats& stats) const noexcept
    {
      return (constructs == stats.constructs) &&
        (destructs == stats.destructs) &&
        (copies == stats.copies) &&
        (moves == stats.moves);
    }

    inline bool operator!=(const Stats& stats) const noexcept
    {
      return !operator==(stats);
    }

    inline bool gotCopies() const noexcept
    {
      return copies > 0;
    }

    inline bool gotMoves() const noexcept
    {
      return moves > 0;
    }
  };
private:

  Stats* stats = nullptr;
  MemorySize id = 0;
  bool moved = false;
public:

  // no default constructor

  explicit Moveable(Stats& _stats)
    : stats(&_stats)
  {
    ++(stats->constructs);
    id = stats->constructs;
  }

  explicit Moveable(Stats* _stats)
    : stats(_stats)
  {
    ++(stats->constructs);
    id = stats->constructs;
  }

  Moveable(const Moveable& copy)
    : stats(copy.stats)
  {
    ++(stats->constructs);
    ++(stats->copies);
    id = stats->constructs;
  }

  Moveable(Moveable&& move) noexcept
    : stats(moveObject(move.stats))
  {
    move.moved = true; // makes it explicitly that object got moved - we need access to stats
    ++(stats->constructs);
    ++(stats->moves);
    id = stats->constructs;
  }

  Moveable& operator=(const Moveable& copy)
  {
    if (&copy != this) {
      stats = copy.stats;
    }
    ++(stats->copies);
    return *this;
  }

  Moveable& operator=(Moveable&& move)
  {
    if (&move != this) {
      stats = moveObject(move.stats);
      move.moved = true; // makes it explicitly that object got moved - we need access to stats
    }
    ++(stats->moves);
    return *this;
  }

  ~Moveable()
  {
    ++(stats->destructs);
  }
};

inline Moveable::Stats operator-(const Moveable::Stats& a, const Moveable::Stats& b) noexcept
{
  Moveable::Stats result;
  result.constructs = a.constructs - b.constructs;
  result.destructs = a.destructs - b.destructs;
  result.copies = a.copies - b.copies;
  result.moves = a.moves - b.moves;
  return result;
}

/** Class used to ensure containers work for non-moveable types. */
class _COM_AZURE_DEV__BASE__API NonMoveable {
public:

  NonMoveable()
  {
  }

  NonMoveable(const NonMoveable& copy)
  {
  }

  NonMoveable& operator=(const NonMoveable& copy)
  {
    return *this;
  }

  NonMoveable(NonMoveable&& move) = delete;
  NonMoveable& operator=(NonMoveable&& move) = delete;

  ~NonMoveable()
  {
  }
};

/** Class used to ensure containers work for non-default constructible types. */
class _COM_AZURE_DEV__BASE__API NonDefaultConstructible {
private:

  NonDefaultConstructible(NonDefaultConstructible&& move) = delete;
  NonDefaultConstructible& operator=(NonDefaultConstructible&& move) = delete;
public:

  explicit NonDefaultConstructible(NullPtr)
  {
  }

  NonDefaultConstructible(const NonDefaultConstructible& copy)
  {
  }

  NonDefaultConstructible& operator=(const NonDefaultConstructible& copy)
  {
    return *this;
  }

  ~NonDefaultConstructible()
  {
  }
};

/** Class used to ensure containers work for default constructible and move assignable types (can be used for fast swap). */
class _COM_AZURE_DEV__BASE__API DefaultAndMoveAssignable {
private:

  DefaultAndMoveAssignable(DefaultAndMoveAssignable&& move) = delete;
public:

  DefaultAndMoveAssignable()
  {
  }

  DefaultAndMoveAssignable(const DefaultAndMoveAssignable& copy)
  {
  }

  DefaultAndMoveAssignable& operator=(const DefaultAndMoveAssignable& copy)
  {
    return *this;
  }

  DefaultAndMoveAssignable& operator=(DefaultAndMoveAssignable&& move)
  {
    return *this;
  }

  ~DefaultAndMoveAssignable()
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
