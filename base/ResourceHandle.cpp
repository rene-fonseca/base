/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ResourceHandle.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

PreferredAtomicCounter ResourceHandle::id; // TAG: share id with Thread?
PreferredAtomicCounter ResourceHandle::total;

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
