/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_REGION_H
#define _DK_SDU_MIP__BASE_IO__FILE_REGION_H

#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Description of a file region represented by an offset (64 bit) and a size (32 bit).

  @short A region of a file.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileRegion : public Object {
private:

  /** The offset of the region from the beginning of the file. */
  long long offset;
  /** The size of the region (in bytes). */
  unsigned int size;
public:

  /**
    Initializes the file region.

    @param offset The offset (from the beginning of the file) of the region.
    @param size The size (in bytes) of the region.
  */
  FileRegion(long long offset, unsigned int size) throw();

  /**
    Initializes region from other region.
  */
  inline FileRegion(const FileRegion& copy) throw() : offset(copy.offset), size(copy.size) {}

  /**
    Assignment of region by region.
  */
  inline FileRegion& operator=(const FileRegion& eq) throw() {
    offset = eq.offset; // no need to protect against self-assignment
    size = eq.size;
    return *this;
  }

  /**
    Returns the offset of the file region.
  */
  inline long long getOffset() const throw() {return offset;}

  /**
    Returns the size of the file region.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Sets the offset of the file region.
  */
  inline void setOffset(long long offset) throw() {this->offset = offset;}

  /**
    Sets the size of the file region.
  */
  inline void setSize(unsigned int size) throw() {this->size = size;}

  /**
    Returns true if the specified region is contained in this region.
  */
  inline bool isWithin(const FileRegion& region) const throw() {
    const long long difference = region.offset - offset;
    return (difference >= 0) && ((difference + region.size - size) <= 0);
  }
};

inline FileRegion::FileRegion(long long o, unsigned int s) throw() : offset(o), size(s) {}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
