/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_DIMENSION_H
#define _BASE_DIMENSION_H

#include "Object.h"

/**
  Dimension.

  Problems: overflow possible in getSize()

  @author René Møller Fonseca
  @version 1.0
*/

class Dimension : public Object, Copyable, Comparable {
protected:

  /** The width. */
  unsigned int width;
  /** The height. */
  unsigned int height;
public:

  /**
    Initialize object. The width and height value are set to zero.
  */
  Dimension() throw();

  /**
    Initialize object. The dimension is copied from the specified dimension.

    @param dimension The desired dimension.
  */
  Dimension(const Dimension& dimension) throw();

  /**
    Initialize object.

    @param width The desired width.
    @param height The desired height.
  */
  Dimension(unsigned int width, unsigned int height) throw();

  /**
    Assigns the dimension.

    @param dimension The desired dimension.
  */
  Dimension& operator=(const Dimension& dimension) throw();

  /**
    Sets the width and the height.

    @param width The desired width.
    @param height The desired height.
  */
  void assign(unsigned int width, unsigned int height) throw();

  /**
    Returns true if the dimensions are equal.

    @param dimension The dimension to be compared.
  */
  bool operator==(const Dimension& dimension) const throw();

  /**
    Returns the width.
  */
  unsigned int getWidth() const throw();

  /**
    Returns the height.
  */
  unsigned int getHeight() const throw();

  /**
    Returns the size (width * height).
  */
  unsigned int getSize() const throw();

  /**
    Sets the width.

    @param width The desired width.
  */
  void setWidth(unsigned int width) throw();

  /**
    Sets the height.

    @param height The desired height.
  */
  void setHeight(unsigned int height) throw();

  /**
    Writes a string representation of this object to stream.
  */
  ostream& operator<<(ostream& stream) const;
};

#endif
