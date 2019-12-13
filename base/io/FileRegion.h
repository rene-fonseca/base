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

#include <base/Object.h>
#include <base/OutOfDomain.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Description of a file region represented by an offset (64 bit) and a size
  (32 bit).

  @short A region of a file.
  @ingroup io
  @version 1.1
*/

class _COM_AZURE_DEV__BASE__API FileRegion : public Object {
private:

  /**
    Specifies the maximum permit able byte offset of any part of the file
    region.
  */
  static const long long LIMIT = PrimitiveTraits<long long>::MAXIMUM;
  
  /** The offset of the region from the beginning of the file. */
  long long offset = 0;
  /** The size of the region (in bytes). */
  unsigned int size = 0;
public:

  /**
    Initializes the file region.

    @param offset The offset (from the beginning of the file) of the region.
    @param size The size (in bytes) of the region.
  */
  FileRegion(long long offset, unsigned int size);

  /**
    Initializes region from other region.
  */
  inline FileRegion(const FileRegion& copy) noexcept
    : offset(copy.offset), size(copy.size)
  {
  }

  /**
    Assignment of region by region.
  */
  inline FileRegion& operator=(const FileRegion& assign) noexcept
  {
    offset = assign.offset; // no need to protect against self-assignment
    size = assign.size;
    return *this;
  }

  /**
    Returns the offset of the file region.
  */
  inline long long getOffset() const noexcept
  {
    return offset;
  }

  /**
    Returns the size of the file region.
  */
  inline unsigned int getSize() const noexcept
  {
    return size;
  }

  /**
    Sets the offset of the file region.
  */
  inline void setOffset(long long offset)
  {
    bassert(offset < LIMIT - size, OutOfDomain(this));
    this->offset = offset;
  }

  /**
    Sets the size of the file region.
  */
  inline void setSize(unsigned int size)
  {
    bassert(offset < LIMIT - size, OutOfDomain(this));
    this->size = size;
  }
  
  /**
    Returns the end of the region.
  */
  inline long long getEnd() const noexcept
  {
    return offset + size;
  }

  /**
    Returns true if the specified region is contained in this region.
  */
  inline bool isWithin(const FileRegion& region) const noexcept
  {
    const long long difference = region.offset - offset;
    return (difference >= 0) && ((difference + region.size - size) <= 0);
  }
};

inline FileRegion::FileRegion(
  long long _offset, unsigned int _size)
  : offset(_offset), size(_size)
{
  bassert(offset < (LIMIT - size), OutOfDomain(this));
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
