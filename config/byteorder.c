/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program returns _DK_SDU_MIP__BASE__BIG_ENDIAN if the byte order of the
  architecture is big endian, _DK_SDU_MIP__BASE__LITTLE_ENDIAN if the byte order
  is little endian, and _DK_SDU_MIP__BASE__UNSPECIFIED if neither.
*/

#include <base/platforms/symbols.h>

int main() {
  union {
    char c[sizeof(unsigned int)];
    unsigned int i;
  } u;

  if (sizeof(unsigned int) > 8) {
    return _DK_SDU_MIP__BASE__UNSPECIFIED;
  }

  u.i = 0;
  for (unsigned int i = sizeof(unsigned int); i > 0; --i) {
    u.i = u.i * 256 + i;
  }

  if (sizeof(unsigned int) == 2) {
    if ((u.c[0] == 2) && (u.c[1] == 1)) {
      return _DK_SDU_MIP__BASE__BIG_ENDIAN;
    } else if ((u.c[0] == 1) && (u.c[1] == 2)) {
      return _DK_SDU_MIP__BASE__LITTLE_ENDIAN;
    }
  } else if (sizeof(unsigned int) == 4) {
    if ((u.c[0] == 4) && (u.c[1] == 3) && (u.c[2] == 2) && (u.c[3] == 1)) {
      return _DK_SDU_MIP__BASE__BIG_ENDIAN;
    } else if ((u.c[0] == 1) && (u.c[1] == 2) && (u.c[2] == 3) && (u.c[3] == 4)) {
      return _DK_SDU_MIP__BASE__LITTLE_ENDIAN;
    }
  } else if (sizeof(unsigned int) == 8) {
    if ((u.c[0] == 8) && (u.c[1] == 7) && (u.c[2] == 6) && (u.c[3] == 5) && (u.c[4] == 4) && (u.c[5] == 3) && (u.c[6] == 2) && (u.c[7] == 1)) {
      return _DK_SDU_MIP__BASE__BIG_ENDIAN;
    } else if ((u.c[0] == 1) && (u.c[1] == 2) && (u.c[2] == 3) && (u.c[3] == 4) && (u.c[4] == 5) && (u.c[5] == 6) && (u.c[6] == 7) && (u.c[7] == 8)) {
      return _DK_SDU_MIP__BASE__LITTLE_ENDIAN;
    }
  }

  return _DK_SDU_MIP__BASE__UNSPECIFIED;
}
