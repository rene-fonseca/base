/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Dimension.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Dimension& value) throw(IOException)
{
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getWidth() << ',' << value.getHeight() << ')';
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
