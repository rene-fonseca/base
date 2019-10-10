/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program returns _COM_AZURE_DEV__BASE__BIG_ENDIAN if the word order of the
  architecture is big endian, _COM_AZURE_DEV__BASE__LITTLE_ENDIAN if the word order
  is little endian, and _COM_AZURE_DEV__BASE__UNSPECIFIED if neither.
*/

#include "floating.h"

int main() {
  // TAG: should check long long (if double word), float, double, and long double?
  int floatWordOrder;
  checkFloat<long double>(floatWordOrder);
  return floatWordOrder;
}
