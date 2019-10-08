/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Dimension represented by width and height.

  @short Dimension
  @version 1.2
*/

class Dimension {
protected:

  /** The width. */
  unsigned int width = 0;
  /** The height. */
  unsigned int height = 0;
public:

  /**
    Initializes object with width and height set to zero.
  */
  inline Dimension() throw() {
  }

  /**
    Initializes object with the specified width and height.

    @param width The desired width.
    @param height The desired height.
  */
  Dimension(unsigned int width, unsigned int height) throw();

  /**
    Initializes object with the dimension copied from the specified dimension.

    @param dimension The desired dimension.
  */
  inline Dimension(const Dimension& copy) throw()
    : width(copy.width), height(copy.height) {
  }

  /**
    Assigns the dimension.

    @param dimension The desired dimension.
  */
  inline Dimension& operator=(const Dimension& eq) throw() {
    width = eq.width; // no need to protect against self-assignment
    height = eq.height;
    return *this;
  }

  /**
    Sets the width and the height.

    @param width The desired width.
    @param height The desired height.
  */
  inline void assign(unsigned int width, unsigned int height) throw() {
    this->width = width;
    this->height = height;
  }

  /**
    Reduces the width and height to the minimum values of this and the specified
    dimension.
    
    @param dimension The other dimension.
  */
  inline void reduce(const Dimension& dimension) throw() {
    if (dimension.getWidth() < width) {
      width = dimension.getWidth();
    }
    if (dimension.getHeight() < height) {
      height = dimension.getHeight();
    }
  }
  
  /**
    Expands the width and height to the maximum value of this and the specified
    dimension.
    
    @param dimension The other dimension.
  */
  inline void expand(const Dimension& dimension) throw() {
    if (dimension.getWidth() > width) {
      width = dimension.getWidth();
    }
    if (dimension.getHeight() > height) {
      height = dimension.getHeight();
    }
  }
  
  /**
    Returns true if the dimensions are equal.

    @param dimension The dimension to be compared.
  */
  inline bool operator==(const Dimension& dimension) const throw() {
    return (width == dimension.width) && (height == dimension.height);
  }
  
  /**
    Returns true if the dimensions are different.

    @param dimension The dimension to be compared.
  */
  inline bool operator!=(const Dimension& dimension) const throw() {
    return (width != dimension.width) || (height != dimension.height);
  }
  
  /**
    Returns true if the dimension is proper (i.e. both the width and height are non-zero).
  */
  inline bool isProper() const throw() {
    return (width != 0) && (height != 0);
  }

  /**
    Returns the width.
  */
  inline unsigned int getWidth() const throw() {
    return width;
  }

  /**
    Returns the height.
  */
  inline unsigned int getHeight() const throw() {
    return height;
  }

  /**
    Returns the size (width * height).
  */
  inline unsigned long long getSize() const throw() {
    return static_cast<unsigned long long>(width) * height;
  }

  /**
    Sets the width.

    @param width The desired width.
  */
  inline void setWidth(unsigned int value) throw() {
    width = value;
  }

  /**
    Sets the height.

    @param value The desired height.
  */
  inline void setHeight(unsigned int value) throw() {
    height = value;
  }
};

inline Dimension::Dimension(unsigned int _width, unsigned int _height) throw()
  : width(_width), height(_height) {
}

/**
  Writes a string representation of a Dimension object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value) throw(IOException);

template<>
class Uninitializeable<Dimension> {
public:

  static const bool IS_UNINITIALIZEABLE = Uninitializeable<unsigned int>::IS_UNINITIALIZEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
