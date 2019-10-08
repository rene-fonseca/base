/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is a wrapper for the 7-bit ASCII octet manipulators and more. This
  class does not depend on the current locale of the application.

  @short ASCII character traits.
  @version 1.2
*/
class _DK_SDU_MIP__BASE__API ASCIITraits {
public:
  
  /** Character characteristics. */
  enum Characteristics {
    LOWER = 1, /**< Any of the characters "abcdefghijklmnopqrstuvwxyz". */
    UPPER = 2, /**< Any of the characters "ABCDEFGHIJKLMNOPQRSTUVWXYZ". */
    ALPHA = LOWER|UPPER, /**< Any upper or lower case letter. */
    DIGIT = 4, /**< Any of the characters "0123456789". */
    ALPHANUM = ALPHA|DIGIT, /**< A letter or digit. */
    OCTAL = 8, /**< Any of the characters "01234567". */
    LOWERHEX = 16, /**< A DIGIT or any of the characters "abcdef". */
    UPPERHEX = 32, /**< A DIGIT or any of the characters "ABCDEF". */
    HEX = LOWERHEX|UPPERHEX, /**< Any upper or lower case hexadicimal digit. */
    CONTROL = 64, /**< The characters from 0x00 to 0x1f and 0x7f. */
    SPACE = 128, /**< Tab, newline, vertical-tab, form-feed, carriage-return, and space. */
    ASCII = 256, /**< The octets from 0x00 to 0x7f. */
    SP = 512, /**< A simple space ' '. */
    PUNCTUATION = 1024, /**< Punctuation characters. */
    GRAPH = ALPHANUM|PUNCTUATION, /**< Visible characters. */
    PRINTABLE = SP|GRAPH, /**< Printable characters. */
  };

  /* Character descriptor. */
  struct CharacterDescriptor {
    /** Character flags. */
    unsigned short flags;
    /** The corresponding lower case character. */
    unsigned char lower;
    /** The corresponding upper case character. */
    unsigned char upper;
    /** The numerical value of the character (hexadecimal). */
    unsigned char value;
  };
private:

  /** Internal lookup table describing all characters (octet). */
  static const CharacterDescriptor* lookup;
public:

  /** Array of all digits (lower case) in ascending order of corresponding value. */
  static const char LOWER_DIGITS[];
  /** Array of all digits (upper case) in ascending order of corresponding value. */
  static const char UPPER_DIGITS[];

  static const char NUL = '\0';
  static const char SOH = 0x01;
  static const char STX = 0x02;
  static const char ETX = 0x03;
  static const char EOT = 0x04;
  static const char ENQ = 0x05;
  static const char ACK = 0x06;
  static const char BEL = '\a';
  static const char BS = '\b';
  static const char HT = '\t';
  static const char LF = '\n';
  static const char VT = '\v';
  static const char FF = '\f';
  static const char CR = '\r';
  static const char SO = 0x0e;
  static const char SI = 0x0f;
  static const char DLE = 0x10;
  static const char DC1 = 0x11;
  static const char DC2 = 0x12;
  static const char DC3 = 0x13;
  static const char DC4 = 0x14;
  static const char NAK = 0x15;
  static const char SYN = 0x16;
  static const char ETB = 0x17;
  static const char CAN = 0x18;
  static const char EM = 0x19;
  static const char SUB = 0x1a;
  static const char ESC = 0x1b;
  static const char FS = 0x1c;
  static const char GS = 0x1d;
  static const char RS = 0x1e;
  static const char US = 0x1f;
  static const char SPC = ' ';
  static const char DEL = 0x7f;
  
  /** Returns true if the character an alphabetic character. */
  static inline bool isAlpha(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & ALPHA;
  }
  
  /** Returns true if the character an alphabetic character or a digit. */
  static inline bool isAlphaNum(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & ALPHANUM;
  }
  
  /** Returns true if the character is lowercase. */
  static inline bool isLower(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & LOWER;
  }
  
  /** Returns true if the character is uppercase. */
  static inline bool isUpper(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & UPPER;
  }
  
  /** Returns true if the character is a digit. */
  static inline bool isDigit(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & DIGIT;
  }
  
  /** Returns true if the character is an octal digit. */
  static inline bool isOctal(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & OCTAL;
  }
  
  /**
    Returns true if the character is a lower hex digit (any of the characters
    "0123456789abcdef").
  */
  static inline bool isLowerHex(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & LOWERHEX;
  }
  
  /**
    Returns true if the character is an upper hex digit (any of the characters
    "0123456789ABCDEF").
  */
  static inline bool isUpperHex(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & UPPERHEX;
  }
  
  /**
    Returns true if the character is a hex digit (either lower or upper hex
    digit).
  */
  static inline bool isHexDigit(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & HEX;
  }
  
  /**
    Returns true if the character is a white space (tab '\\t', newline '\\n',
    vertical-tab '\\v', form-feed '\\f', carriage-return '\\r', and space ' ').
  */
  static inline bool isSpace(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & SPACE;
  }
  
  /** Returns true if the character is a punctuation mark. */
  static inline bool isPunctuation(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & PUNCTUATION;
  }
  
  /** Returns true if the character is printable. */
  static inline bool isPrintable(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & PRINTABLE;
  }
  
  /** Returns true if the character is a visible character. */
  static inline bool isGraph(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & GRAPH;
  }
  
  /**
    Returns true if the character is a control character (the octets from 0x00
    to 0x1f).
  */
  static inline bool isControl(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].flags & CONTROL;
  }
  
  /**
    Returns true if the character is an ASCII character (i.e. the octets from
    0x00 to 0x7f).
  */
  static inline bool isASCII(char value) throw() {
    return !(value & ~0177U);
  }

  /**
    Returns the value of the specified character. The character must be a digit
    approved by isDigit(), isOctal(), isLowerHex(), isUpperHex(), or
    isHexDigit(). If the character is not a digit an unspecified value is
    returned.
  */
  static inline unsigned char digitToValue(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].value;
  }

  /**
    Returns the character representation of the specified value. An unspecified
    value is returned if the argument falls outside the range [0..15].
  */
  static inline char valueToDigit(unsigned int value) throw() {
    return LOWER_DIGITS[value & 0x0f];
  }

  /**
    Returns the character representation of the specified value. An unspecified
    value is returned if the argument falls outside the range [0..15].

    @param value The value of the digit.
    @param upper Selects upper case if true otherwise lower case.
  */
  static inline char valueToDigit(unsigned int value, bool upper) throw() {
    return upper ? UPPER_DIGITS[value & 0x0f] : LOWER_DIGITS[value & 0x0f];
  }

  /**
    Returns the character representation of the specified value. An unspecified
    value is returned if the argument falls outside the range [0..15].
  */
  static inline char valueToLowerDigit(unsigned int value) throw() {
    return LOWER_DIGITS[value & 0x0f];
  }

  /**
    Returns the character representation of the specified value. An unspecified
    value is returned if the argument falls outside the range [0..15].
  */
  static inline char valueToUpperDigit(unsigned int value) throw() {
    return UPPER_DIGITS[value & 0x0f];
  }

  /** Returns the lower case representation of the character. */
  static inline char toLower(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].lower;
  }
  
  /** Returns the upper case representation of the character. */
  static inline char toUpper(char value) throw() {
    return lookup[static_cast<unsigned char>(value)].upper;
  }
  
  /**
    Compares the specified characters (less than (negative), equal (0), and
    greater than (positive).
  */
  static inline int compare(char left, char right) throw() {
    return static_cast<int>(left) - static_cast<int>(right);
  }
  
  /**
    Compares the specified characters ignoring the case (less than (negative),
    equal (0), and greater than (positive).
  */
  static inline int compareCaseless(char left, char right) throw() {
    return static_cast<int>(lookup[static_cast<unsigned char>(left)].lower) -
      static_cast<int>(lookup[static_cast<unsigned char>(right)].lower);
  }

  /**
    Unary function object which returns the corresponding lower case of the
    specified character.

    @short Lower case conversion function object.
  */
  class ToLowerCase {
  public:
    
    inline char operator()(char value) const throw() {
      return lookup[static_cast<unsigned char>(value)].lower;
    }
  };

  /**
    Unary function object which returns the corresponding upper case of the
    specified character.
    
    @short Upper case conversion function object.
  */
  class ToUpperCase {
  public:
    
    inline char operator()(char value) const throw() {
      return lookup[static_cast<unsigned char>(value)].upper;
    }
  };

  /**
    Binary function object which compares the specified characters.

    @short Comparison of characters function object.
  */
  class Compare {
  public:
    
    inline int operator()(char left, char right) const throw() {
      return compare(left, right);
    }
  };

  /**
    Binary function object which compares the specified characters.
    
    @short Case-less comparison of characters function object.
  */
  class CompareCaseless {
  public:
    
    inline int operator()(char left, char right) const throw() {
      return compareCaseless(left, right);
    }
  };
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
