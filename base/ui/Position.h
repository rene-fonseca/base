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

#include <base/Dimension.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

  /**
    A two-dimensional discrete coordinate.

    @short A two-dimensional coordinate.
    @version 1.1
  */
  
class _COM_AZURE_DEV__BASE__API Position : public Object {
private:
  
  /** The x coordinate. */
  int x = 0;
  /** The y coordinate. */
  int y = 0;
public:

  /**
    Initializes position as (0, 0).
  */
  inline Position() noexcept {
  }

  /**
    Initializes position with specified x and y values.
  */
  inline Position(int _x, int _y) noexcept : x(_x), y(_y) {
  }

  /**
    Initializes position from dimension (width, height) as (width - 1, height - 1).
  */
  inline Position(const Dimension& dimension) noexcept
    : x(dimension.getWidth()),
      y(dimension.getHeight()) {
    if (x > 0) {
      --x;
    }
    if (y > 0) {
      --y;
    }
  }
  
  /**
    Initializes position from other position.
  */
  inline Position(const Position& copy) noexcept : x(copy.x), y(copy.y) {
  }

  /**
    Assignment of position by position.
  */
  inline Position& operator=(const Position& assign) noexcept {
    x = assign.x;
    y = assign.y;
    return *this;
  }

  /**
    Returns true if position is (0, 0).
  */
  inline bool isOrigin() const noexcept {
    return (x == 0) && (y == 0);
  }
  
  /**
    Returns true if the position is contained in the bounding rectangle
    specified by the upper left and lower right corners.
    
    @param upperLeft The upper left corner.
    @param lowerRight The lower right corner.
  */
  inline bool isWithin(const Position& upperLeft, const Position& lowerRight) const noexcept {
    return (x >= upperLeft.x) &&
      (y >= upperLeft.y) &&
      (x <= lowerRight.x) &&
      (y <= lowerRight.y);
  }

  /**
    Returns true if the position is contained in the bounding rectangle
    specified by the upper left corner and the dimension.
    
    @param position Upper left corner of the bounding rectangle.
    @param dimension Dimension of the bounding rectangle.
  */
  inline bool isWithin(const Position& position, const Dimension& dimension) const noexcept {
    return (x >= position.x) &&
      (y >= position.y) &&
      (x < static_cast<int>(position.x + dimension.getWidth())) &&
      (y < static_cast<int>(position.y + dimension.getHeight()));
  }
  
  /**
    Returns true if the position is contained in the bounding rectangle given by
    the upper left corner (0, 0) and the specified dimension.
    
    @param dimension Dimension of the bounding rectangle.
  */
  inline bool isWithin(const Dimension& dimension) const noexcept {
    return (x >= 0) &&
      (y >= 0) &&
      (static_cast<unsigned int>(x) < dimension.getWidth()) &&
      (static_cast<unsigned int>(y) < dimension.getHeight());
  }

  /**
    Confines the position to the specified rectangle.
    
    @param a Corner.
    @param b Corner.

    @return Returns true if position was NOT changed.
  */
  inline bool confineTo(const Position& a, const Position& b) noexcept {
    bool ok = true;
    if (x < minimum(a.x, b.x)) {
      x = minimum(a.x, b.x);
      ok = false;
    } else if (x > maximum(a.x, b.x)) {
      x = maximum(a.x, b.x);
      ok = false;
    }
    if (y < minimum(a.y, b.y)) {
      y = minimum(a.y, b.y);
      ok = false;
    } else if (y > maximum(a.y, b.y)) {
      y = maximum(a.y, b.y);
      ok = false;
    }
    return ok;
  }  
  
  /**
    Returns the X coordinate.
  */
  inline int getX() const noexcept {
    return x;
  }

  /**
    Returns the Y coordinate.
  */
  inline int getY() const noexcept {
    return y;
  }

  /**
    Sets the X coordinate.
  */
  inline void setX(int x) noexcept {
    this->x = x;
  }

  /**
    Sets the Y coordinate.
  */
  inline void setY(int y) noexcept {
    this->y = y;
  }

  /**
    Negates this vector.
  */
  inline Position& negate() noexcept {
    x = -x;
    y = -y;
    return *this;
  }

  /**
    Adds the specified position to this position.
  */
  inline Position& add(const Position& value) noexcept {
    x += value.x;
    y += value.y;
    return *this;
  }

  /**
    Subtracts the specified position from this position.
  */
  inline Position& subtract(const Position& value) noexcept {
    x -= value.x;
    y -= value.y;
    return *this;
  }

  /**
    Returns true if the positions are equal.

    @param position The position to be compared.
  */
  inline bool operator==(const Position& position) const noexcept {
    return (x == position.x) && (y == position.y);
  }
  
  /**
    Returns true if the positions are different.

    @param position The position to be compared.
  */
  inline bool operator!=(const Position& position) const noexcept {
    return (x != position.x) || (y != position.y);
  }

  /**
    Adds the specified position from this position.

    @param value The value to be added.
  */
  inline Position& operator+=(const Position& value) noexcept {
    return add(value);
  }

  /**
    Subtracts the specified position from this position.

    @param value The value to be subtracted.
  */
  inline Position& operator-=(const Position& value) noexcept {
    return subtract(value);
  }
  
  /**
    Subtracts the position from this position.
  */
  inline Position& operator-(const Position& position) noexcept {
    x -= position.x;
    y -= position.y;
    return *this;
  }
  
  /**
    Adds the position from this position.
  */
  inline Position& operator+(const Position& position) noexcept {
    x += position.x;
    y += position.y;
    return *this;
  }

  /**
    Returns true if the position is after the specified position.
  */
  inline bool isAfter(const Position& position) const noexcept {
    return (x > position.x) && (y > position.y);
  }
  
  /**
    Returns true if the position is before the specified position.
  */
  inline bool isBefore(const Position& position) const noexcept {
    return (x < position.x) || (y < position.y);
  }
  
  /**
    Returns the dimension of the rectangle from this position (upper left) to
    the specified lower right corner.
    
    @param position The lower right corner.
  */
  inline Dimension getDimension(const Position& position) const noexcept {
    const Position temp(position.x - x, position.y - y);
    return Dimension(maximum(temp.x, 0), maximum(temp.y, 0));
  }
  
  /**
    Returns the dimension of the rectangle defined by the 2 positions. 

    @param upperLeft The upper left or lower right corner.
    @param lowerRight The upper left or lower right corner.
  */
  static inline Dimension getDimension(const Position& upperLeft, const Position& lowerRight) noexcept {
    const Position position(upperLeft.x - lowerRight.x, upperLeft.y - lowerRight.y);
    return Dimension(
      maximum(position.x, -position.x),
      maximum(position.y, -position.y)
    );
  }
};

inline Position operator-(const Position& left, const Position& right) noexcept {
  return Position(left.getX() - right.getX(), left.getY() - right.getY());
}

inline Position operator+(const Position& left, const Position& right) noexcept {
  return Position(left.getX() + right.getX(), left.getY() + right.getY());
}

/**
  Writes a string representation of a Position object to a format stream. The format is "(x, y)".
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Position& value);

template<>
class IsUninitializeable<Position> : public IsUninitializeable<int> {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
