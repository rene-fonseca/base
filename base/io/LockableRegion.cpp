/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/LockableRegion.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

LockableRegion::LockableRegion(
  const File& _file,
  const FileRegion& _region,
  bool exclusive) throw(FileException) : file(_file), region(_region) {
  file.lock(region, exclusive);
}

void LockableRegion::lock(
  const FileRegion& region,
  bool exclusive) throw(FileException) {
  file.unlock(this->region);
  this->region = region;
  file.lock(this->region, exclusive);
}

LockableRegion::~LockableRegion() throw(FileException) {
  file.unlock(region);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
