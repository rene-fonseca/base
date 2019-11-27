/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Range2D.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, const Range2D& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getMinimum() << ',' << value.getMaximum() << ')';
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
