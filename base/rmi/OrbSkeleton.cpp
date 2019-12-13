/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbSkeleton.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OrbSkeleton::OrbSkeleton(const String& _name, unsigned int _version)
  : name(_name), version(_version) {
}

OrbSkeleton::~OrbSkeleton() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
