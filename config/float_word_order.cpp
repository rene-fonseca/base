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
  This program returns _DK_SDU_MIP__BASE__BIG_ENDIAN if the word order of the
  architecture is big endian, _DK_SDU_MIP__BASE__LITTLE_ENDIAN if the word order
  is little endian, and _DK_SDU_MIP__BASE__UNSPECIFIED if neither.
*/

#include "floating.h"

int main() {
  // TAG: should check long long (if double word), float, double, and long double?
  int floatWordOrder;
  checkFloat<long double>(floatWordOrder);
  return floatWordOrder;
}
