/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DIMENSION_H
#define _DK_SDU_MIP__BASE__DIMENSION_H

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Dimension represented by width and height.

  @author René Møller Fonseca
  @version 1.01
*/

class Dimension : public virtual Object {
protected:

  /** The width. */
  unsigned int width;
  /** The height. */
  unsigned int height;
public:

  /**
    Initializes object with width and height set to zero.
  */
  inline Dimension() throw() : width(0), height(0) {}

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
  inline Dimension(const Dimension& copy) throw() : width(copy.width), height(copy.height) {}

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
    Returns true if the dimensions are equal.

    @param dimension The dimension to be compared.
  */
  inline bool operator==(const Dimension& dimension) const throw() {
    return (width == dimension.width) && (height == dimension.height);
  }

  /**
    Returns true if the dimension is proper (i.e. both the width and height are non-zero).
  */
  inline bool isProper() const throw() {return (width != 0) && (height != 0);}

  /**
    Returns the width.
  */
  inline unsigned int getWidth() const throw() {return width;}

  /**
    Returns the height.
  */
  inline unsigned int getHeight() const throw() {return height;}

  /**
    Returns the size (width * height).
  */
  inline unsigned long long getSize() const throw() {return width * height;}

  /**
    Sets the width.

    @param width The desired width.
  */
  inline void setWidth(unsigned int value) throw() {width = value;}

  /**
    Sets the height.

    @param value The desired height.
  */
  inline void setHeight(unsigned int value) throw() {height = value;}

  /**
    Writes a string representation of a Dimension object to a format stream. The format is "(width, height)".
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value);
};

inline Dimension::Dimension(unsigned int w, unsigned int h) throw() : width(w), height(h) {}

/**
  Writes a string representation of a Dimension object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value);

template<>
inline bool isRelocateable<Dimension>() throw() {return isRelocateable<Object>();}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
