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
  This program returns _COM_AZURE_DEV__BASE__BIG_ENDIAN if the byte order of the
  architecture is big endian, _COM_AZURE_DEV__BASE__LITTLE_ENDIAN if the byte order
  is little endian, and _COM_AZURE_DEV__BASE__UNSPECIFIED if neither.
*/

#include <base/platforms/symbols.h>

int main() {
  union {
    char c[sizeof(unsigned int)];
    unsigned int i;
  } u;
  unsigned int j;

  if (sizeof(unsigned int) > 8) {
    return _COM_AZURE_DEV__BASE__UNSPECIFIED;
  }

  u.i = 0;
  for (j = sizeof(unsigned int); j > 0; --j) {
    u.i = u.i * 256 + j;
  }

  if (sizeof(unsigned int) == 2) {
    if ((u.c[0] == 2) && (u.c[1] == 1)) {
      return _COM_AZURE_DEV__BASE__BIG_ENDIAN;
    } else if ((u.c[0] == 1) && (u.c[1] == 2)) {
      return _COM_AZURE_DEV__BASE__LITTLE_ENDIAN;
    }
  } else if (sizeof(unsigned int) == 4) {
    if ((u.c[0] == 4) && (u.c[1] == 3) && (u.c[2] == 2) && (u.c[3] == 1)) {
      return _COM_AZURE_DEV__BASE__BIG_ENDIAN;
    } else if ((u.c[0] == 1) && (u.c[1] == 2) && (u.c[2] == 3) && (u.c[3] == 4)) {
      return _COM_AZURE_DEV__BASE__LITTLE_ENDIAN;
    }
  } else if (sizeof(unsigned int) == 8) {
    if ((u.c[0] == 8) && (u.c[1] == 7) && (u.c[2] == 6) && (u.c[3] == 5) && (u.c[4] == 4) && (u.c[5] == 3) && (u.c[6] == 2) && (u.c[7] == 1)) {
      return _COM_AZURE_DEV__BASE__BIG_ENDIAN;
    } else if ((u.c[0] == 1) && (u.c[1] == 2) && (u.c[2] == 3) && (u.c[3] == 4) && (u.c[4] == 5) && (u.c[5] == 6) && (u.c[6] == 7) && (u.c[7] == 8)) {
      return _COM_AZURE_DEV__BASE__LITTLE_ENDIAN;
    }
  }

  return _COM_AZURE_DEV__BASE__UNSPECIFIED;
}
