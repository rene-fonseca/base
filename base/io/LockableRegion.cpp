/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/LockableRegion.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

LockableRegion::LockableRegion(
  const File& _file,
  const FileRegion& _region,
  bool exclusive) : file(_file), region(_region) {
  file.lock(region, exclusive);
}

void LockableRegion::lock(
  const FileRegion& region,
  bool exclusive) {
  file.unlock(this->region);
  this->region = region;
  file.lock(this->region, exclusive);
}

LockableRegion::~LockableRegion() {
  file.unlock(region);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
