/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/Complex.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const Complex Complex::ZERO(0, 0);
const Complex Complex::ONE(1, 0);
const Complex Complex::MINUS_ONE(-1, 0);
const Complex Complex::II(0, 1);

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Complex& value) throw(IOException)
{
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getReal() << FPLUS << value.getImaginary() << "i)";
  // return stream << '(' << value.getReal() << ';' << value.getImaginary() << ')';
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
