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

#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Dimension represented by width and height.

  @short Dimension
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Dimension {
protected:

  /** The width. */
  unsigned int width = 0;
  /** The height. */
  unsigned int height = 0;
public:

  /**
    Initializes object with width and height set to zero.
  */
  inline Dimension() noexcept
  {
  }

  /**
    Initializes object with the specified width and height.

    @param width The desired width.
    @param height The desired height.
  */
  Dimension(unsigned int width, unsigned int height) noexcept;

  /**
    Initializes object with the dimension copied from the specified dimension.

    @param dimension The desired dimension.
  */
  inline Dimension(const Dimension& copy) noexcept
    : width(copy.width), height(copy.height)
  {
  }

  /**
    Assigns the dimension.

    @param dimension The desired dimension.
  */
  inline Dimension& operator=(const Dimension& assign) noexcept
  {
    width = assign.width; // no need to protect against self-assignment
    height = assign.height;
    return *this;
  }

  /**
    Sets the width and the height.

    @param width The desired width.
    @param height The desired height.
  */
  inline void assign(unsigned int width, unsigned int height) noexcept
  {
    this->width = width;
    this->height = height;
  }

  /**
    Reduces the width and height to the minimum values of this and the specified
    dimension.
    
    @param dimension The other dimension.
  */
  inline void reduce(const Dimension& dimension) noexcept
  {
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
  inline void expand(const Dimension& dimension) noexcept
  {
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
  inline bool operator==(const Dimension& dimension) const noexcept
  {
    return (width == dimension.width) && (height == dimension.height);
  }
  
  /**
    Returns true if the dimensions are different.

    @param dimension The dimension to be compared.
  */
  inline bool operator!=(const Dimension& dimension) const noexcept
  {
    return (width != dimension.width) || (height != dimension.height);
  }
  
  /**
    Returns true if the dimension is proper (i.e. both the width and height are non-zero).
  */
  inline bool isProper() const noexcept
  {
    return (width != 0) && (height != 0);
  }

  /**
    Returns the width.
  */
  inline unsigned int getWidth() const noexcept
  {
    return width;
  }

  /**
    Returns the height.
  */
  inline unsigned int getHeight() const noexcept
  {
    return height;
  }

  /**
    Returns the size (width * height).
  */
  inline unsigned long long getSize() const noexcept
  {
    return static_cast<unsigned long long>(width) * height;
  }

  /**
    Sets the width.

    @param width The desired width.
  */
  inline void setWidth(unsigned int value) noexcept
  {
    width = value;
  }

  /**
    Sets the height.

    @param value The desired height.
  */
  inline void setHeight(unsigned int value) noexcept
  {
    height = value;
  }
};

inline Dimension::Dimension(unsigned int _width, unsigned int _height) noexcept
  : width(_width), height(_height)
{
}

/**
  Writes a string representation of a Dimension object to a format stream.
*/
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value);

template<>
class IsUninitializeable<Dimension> : public IsUninitializeable<unsigned int> {
};

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE(Dimension)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
