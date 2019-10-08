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
  Links if mbrtowc is available.
*/

#include <wchar.h>

int main() {
  wchar_t dest[256];
  const char* src = "My string";
  mbstate_t state;

  size_t result = mbrtowc(dest, &src, 256, &state);

  return 0;
}
