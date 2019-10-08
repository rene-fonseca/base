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
  This program compiles/links if the wide-character interface is available.
*/

#include <wchar.h>
#include <wctype.h>

int main() {
  size_t mySize;
  mbstate_t myState;
  wchar_t myCharacter = L'A';

  iswalnum(myCharacter);
  iswalpha(myCharacter);
  iswcntrl(myCharacter);
  iswdigit(myCharacter);
  iswgraph(myCharacter);
  iswlower(myCharacter);
  iswprint(myCharacter);
  iswpunct(myCharacter);
  iswspace(myCharacter);
  iswupper(myCharacter);
  iswxdigit(myCharacter);

  towlower(myCharacter);
  towupper(myCharacter);

  wcslen(L"My wide-character string");
  wcschr(L"My wide-character string", myCharacter);
  wcscmp(L"left", L"right");
  wcsncmp(L"left", L"right", 1);

  //mbsinit(&myState);
  //mySize = mbrlen("My multi-byte string", sizeof("My multi-byte string"), &myState);

  return 0;
}
