/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_OPENGL__POSITION_H
#define _DK_SDU_MIP__BASE_OPENGL__POSITION_H

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

  /**
    A two-dimensional discrete coordinate.

    @short A two-dimensional coordinate.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  
class Position : public Object {
private:
  
  /** The x coordinate. */
  int x;
  /** The y coordinate. */
  int y;
public:

  /**
    Initializes position as (0, 0).
  */
  inline Position() throw() : x(0), y(0) {
  }

  /**
    Initializes position with specified x and y values.
  */
  inline Position(int _x, int _y) throw() : x(_x), y(_y) {
  }
  
  /**
    Initializes position from other position.
  */
  inline Position(const Position& copy) throw() : x(copy.x), y(copy.y) {
  }

  /**
    Assignment of position by position.
  */
  inline Position& operator=(const Position& eq) throw() {
    x = eq.x;
    y = eq.y;
    return *this;
  }

  /**
    Returns true if position is (0, 0).
  */
  inline bool isOrigin() const throw() {
    return (x == 0) && (y == 0);
  }

  /**
    Returns the X coordinate.
  */
  inline int getX() const throw() {
    return x;
  }

  /**
    Returns the Y coordinate.
  */
  inline int getY() const throw() {
    return y;
  }

  /**
    Sets the X coordinate.
  */
  inline void setX(int x) throw() {
    this->x = x;
  }

  /**
    Sets the Y coordinate.
  */
  inline void setY(int y) throw() {
    this->y = y;
  }

  /**
    Subtracts the position from this position.
  */
  Position& operator-(const Position& position) throw() {
    x -= position.getX();
    y -= position.getY();
    return *this;
  }
  
  /**
    Adds the position from this position.
  */
  Position& operator+(const Position& position) throw() {
    x += position.getX();
    y += position.getY();
    return *this;
  }
};

inline Position operator-(const Position& left, const Position& right) throw() {
  return Position(left.getX() - right.getX(), left.getY() - right.getY());
}

inline Position operator+(const Position& left, const Position& right) throw() {
  return Position(left.getX() + right.getX(), left.getY() + right.getY());
}

/**
  Writes a string representation of a Position object to a format stream. The format is "(x, y)".
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Position& value) throw(IOException);

template<>
class Relocateable<Position> {
public:

  static const bool IS_RELOCATEABLE = Relocateable<int>::IS_RELOCATEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
