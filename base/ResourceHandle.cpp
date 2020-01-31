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

FormatOutputStream& operator<<(FormatOutputStream& stream, const ResourceHandle& handle)
{
  auto description = handle.getDescription();
  stream << "Resource: {" << EOL
    << indent(2) << "ID: " << handle.getResourceId() << EOL
    // C++: ?: operator would be convenient value e.g. (getDescription() ?: "<NONE>")
    << indent(2) << "Description: " << (description ? description : "<NONE>") << EOL
    << indent(2) << "Created by: " << handle.getCreatedById() << EOL
    << '}';
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
