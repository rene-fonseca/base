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
  This program returns 0 if the byte order of the architecture is big
  endian otherwise 1 is returned (little endian).
*/

int main() {
  union {
    char c[sizeof(int)];
    int i;
  } u;
  u.i = 1;
  return u.c[0];
}
