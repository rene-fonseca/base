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
  Returns 1 if the word order is big endian otherwise 0 (little endian).
*/

#include "floating.h"

int main() {
  // TAG: should check float, double, and long double
  int floatWordOrder;
  checkFloat<long double>(floatWordOrder);
  return floatWordOrder;
}
