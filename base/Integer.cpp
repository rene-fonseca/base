/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Integer.h"

void Integer::fromString(const String& str) throw(InvalidFormat, OutOfRange) {
/*
  unsigned int index = 0;
  unsigned int size = str.size();
  int value = 0;
  char ch;

  while (index < size) {
    ch = str[index];
    if ((ch >= '0') && (ch <= '9')) {

      int digit = (unsigned int)(ch - '0');
      if (value == INT_MAX/10) {
        if (digit > INT_MAX % 10) {
          throw OutOfRange();
        }
      } else if (value > INT_MAX/10) {
        throw OutOfRange();
      }
      value = value * 10 + digit;

    } else {
      throw InvalidFormat();
    }
  }

  this->value = value;
*/
}

StringOutputStream& operator<<(StringOutputStream& stream, int value) {
  typedef enum {BIN, OCT, DEC, HEX} Base;
  Base base = DEC;
  bool prefix = true;
  bool reverse = false;

  static const char DIGITS[] = "0123456789abcdef";
  char buffer[34]; // longest string for binary format with prefix and terminator
  char* dest = (char*)&buffer;
  int temp = (value >= 0) ? value : -value;

  switch (base) {
  case BIN: // no sign
    if (prefix) {
      *dest++ = '%';
    }
    for (unsigned int i = 31; i >= 0; --i) {
      *dest++ = DIGITS[((unsigned int)value >> i) & 0x00000001];
    }
    break;
  case OCT: // 00 if zero and prefix
    {
      unsigned int temp = (value >= 0) ? value : -value;
      if (value < 0) { // is negative
        *dest++ = '-'; // add sign
      }
      if (prefix) {
        *dest++ = '0'; // add prefix
      }
      bool first = true;
      for (unsigned int i = 30; i >= 0; i -= 3) {
        unsigned int digit = ((unsigned int)value >> i) & 0x00000007;
        if ((digit != 0) || (!first)) {
          first = false;
          *dest++ = DIGITS[digit];
        }
      }
      if (first) { // no digits if value = 0
        *dest++ = DIGITS[0];
      }
      break;
    }
  case DEC: // has no prefix
    {
      unsigned int temp = (value >= 0) ? value : -value;
      unsigned int divisor = 1000000000;
      bool first = true;
      if (value < 0) { // is negative
        *dest++ = '-'; // add sign
      }
      while (divisor > 0) {
        int digit = (temp/divisor) % 10;
        if ((digit != 0) || (!first)) {
          first = false;
          *dest++ = DIGITS[digit];
        }
        divisor /= 10;
      };
      if (first) { // no digits if value = 0
        *dest++ = DIGITS[0];
      }
      break;
    }
  case HEX: // no sign
    if (prefix) {
      *dest++ = '0';
      *dest++ = 'x';
    }
    for (unsigned int i = 28; i >= 0; i -= 4) {
      *dest++ = DIGITS[((unsigned int)value >> i) & 0x0000000f];
    }
    break;
  }
  return stream;
}
