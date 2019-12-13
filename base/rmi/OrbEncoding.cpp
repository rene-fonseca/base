/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbEncoding.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String OrbEncoding::getUrnNamespace() noexcept {
  return MESSAGE("urn:rmi:orb:encoding-scheme:");
}

OrbEncoding::~OrbEncoding() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
