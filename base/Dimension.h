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
  Dimension.

  @author René Møller Fonseca
  @version 1.0
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
  Dimension() throw();

  /**
    Initializes object with the dimension copied from the specified dimension.

    @param dimension The desired dimension.
  */
  Dimension(const Dimension& dimension) throw();

  /**
    Initializes object with the specified width and height.

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
    Writes a string representation of a Dimension object to a format stream. The format is "(width, height)".
  */
  friend FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value);
};

/**
  Writes a string representation of a Dimension object to a format stream.
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
