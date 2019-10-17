/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Primitives.h>
#include <float.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const float PrimitiveTraits<float>::MINIMUM = FLT_MIN;
const float PrimitiveTraits<float>::MAXIMUM = FLT_MAX;

const double PrimitiveTraits<double>::MINIMUM = DBL_MIN;
const double PrimitiveTraits<double>::MAXIMUM = DBL_MAX;

const long double PrimitiveTraits<long double>::MINIMUM = DBL_MIN;
const long double PrimitiveTraits<long double>::MAXIMUM = DBL_MAX;

MemorySize NativeString::getLength() const throw() {
  MemorySize length = 0;
  if (value) {
    const char* src = value;
    while (*src++) {
      ++length;
    }
  }
  return length;
}

MemorySize NativeWideString::getLength() const throw() {
  MemorySize length = 0;
  if (value) {
    const wchar* src = value;
    while (*src++) {
      ++length;
    }
    return 0;
  }
  return length;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
