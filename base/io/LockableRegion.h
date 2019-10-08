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
#include <base/io/File.h>
#include <base/io/FileRegion.h>
#include <base/io/FileException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A lockable file region.
  
  @short A lockable file region.
  @ingroup io
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API LockableRegion : public Object {
private:

  /** The file. */
  File file;
  /** The region. */
  FileRegion region;
public:

  /**
    Initializes the file region lock.

    @param file The file.
    @param region The file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively (if
    false the region is locked shared). This argument is true by default.
  */
  LockableRegion(const File& file, const FileRegion& region, bool exclusive) throw(FileException);

  /**
    Returns the locked file region.
  */
  inline const FileRegion& getRegion() const throw() {
    return region;
  }

  /**
    Lock the specified file region (the old region is unlocked first).

    @param region The new file region to be locked.
    @param exclusive Specifies that the region should be locked exclusively (if
    false the region is locked shared). This argument is true by default.
  */
  void lock(const FileRegion& region, bool exclusive = true) throw(FileException);

  /**
    Destroys the file region lock (unlocks the region).
  */
  ~LockableRegion();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
