/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__BASE64_H
#define _DK_SDU_MIP__BASE_SECURITY__BASE64_H

#include <base/Object.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Base64 encoding (see RFC ???).

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Base64 : public Object {
public:

  /** Digits of the Base64 encoding. */
  static const char DIGITS[];
  /** The special character used to denote unused digits. */
  static const char UNUSED = '=';

  /**
    Returns the digit for the specified value. The value returned is undefined
    if the value is not in the range [0;63].
  */
  static inline char valueToDigit(unsigned int value) throw() {return DIGITS[value & 0x3f];}

  /**
    Returns the value [0;63] of the specified digit.

    @return -1 if not a valid Base64 digit.
  */
  static int valueToDigit(char digit) throw();

  /**
    Returns the Base64 encoded data.

    @param buffer Buffer holding the data.
    @param size The number of octets in the buffer.
  */
  static String encode(const byte* buffer, unsigned int size) throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
