/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program links if memset is provided by the c library.
*/

typedef unsigned int size_t;

#if defined(__cplusplus)
extern "C" {
#endif
  void* memset(void*, int, size_t);
#if defined(__cplusplus)
}
#endif

int main() {
  memset(0, 0, 0);
  return 0;
}
