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
  This program returns 0 if the char primitive is unsigned and 1 if
  it is signed.
*/

int main() {
  char value = 127; // 127 (0x7f) is valid for both signed and unsigned
  ++value; // -128 (0x80) for signed and 128 (0x80) for unsigned
  return (static_cast<int>(value) == 128) ? 0 : 1; // sign-extend value
}
