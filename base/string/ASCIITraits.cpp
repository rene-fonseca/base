/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/ASCIITraits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

const char ASCIITraits::LOWER_DIGITS[] = "0123456789abcdef";
const char ASCIITraits::UPPER_DIGITS[] = "0123456789ABCDEF";

namespace ASCIITraitsImpl {
  
  const ASCIITraits::CharacterDescriptor lookup[256] = {
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x00, 0x00},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x01, 0x01},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x02, 0x02},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x03, 0x03},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x04, 0x04},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x05, 0x05},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x06, 0x06},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x07, 0x07},

    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x08, 0x08}, // bell '\a'
    {ASCIITraits::ASCII | ASCIITraits::CONTROL | ASCIITraits::SPACE, 0x09, 0x09}, // horizontal tab '\t'
    {ASCIITraits::ASCII | ASCIITraits::CONTROL | ASCIITraits::SPACE, 0x0a, 0x0a}, // line-feed '\n'
    {ASCIITraits::ASCII | ASCIITraits::CONTROL | ASCIITraits::SPACE, 0x0b, 0x0b}, // vertical tab '\v'
    {ASCIITraits::ASCII | ASCIITraits::CONTROL | ASCIITraits::SPACE, 0x0c, 0x0c}, // form-feed '\f'
    {ASCIITraits::ASCII | ASCIITraits::CONTROL | ASCIITraits::SPACE, 0x0d, 0x0d}, // carriage-return '\r'
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x0e, 0x0e},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x0f, 0x0f},

    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x10, 0x10},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x11, 0x11},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x12, 0x12},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x13, 0x13},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x14, 0x14},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x15, 0x15},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x16, 0x16},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x17, 0x17},

    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x18, 0x18},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x19, 0x19},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x1a, 0x1a},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x1b, 0x1b},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x1c, 0x1c},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x1d, 0x1d},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x1e, 0x1e},
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x1f, 0x1f},

    {ASCIITraits::ASCII | ASCIITraits::SPACE | ASCIITraits::SP, ' ', ' '}, // space
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '!', '!'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '"', '"'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '#', '#'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '$', '$'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '%', '%'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '&', '&'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '\'', '\''},

    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '(', '('},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, ')', ')'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '*', '*'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '+', '+'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, ',', ','}, // comma
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '-', '-'}, // hyphen
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '.', '.'}, // period
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '/', '/'}, // slash

    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '0', '0', 0}, // 0x30
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '1', '1', 1},
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '2', '2', 2},
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '3', '3', 3},
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '4', '4', 4},
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '5', '5', 5},
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '6', '6', 6},
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::OCTAL | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '7', '7', 7},

    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '8', '8', 8}, // 0x38
    {ASCIITraits::ASCII | ASCIITraits::DIGIT | ASCIITraits::LOWERHEX | ASCIITraits::UPPERHEX, '9', '9', 9},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, ':', ':'}, // colon
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, ';', ';'}, // semicolon
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '<', '<'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '=', '='}, // equals sign
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '>', '>'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '?', '?'}, // question mark

    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '@', '@'}, // 0x40
    {ASCIITraits::ASCII | ASCIITraits::UPPER | ASCIITraits::UPPERHEX, 'a', 'A', 10},
    {ASCIITraits::ASCII | ASCIITraits::UPPER | ASCIITraits::UPPERHEX, 'b', 'B', 11},
    {ASCIITraits::ASCII | ASCIITraits::UPPER | ASCIITraits::UPPERHEX, 'c', 'C', 12},
    {ASCIITraits::ASCII | ASCIITraits::UPPER | ASCIITraits::UPPERHEX, 'd', 'D', 13},
    {ASCIITraits::ASCII | ASCIITraits::UPPER | ASCIITraits::UPPERHEX, 'e', 'E', 14},
    {ASCIITraits::ASCII | ASCIITraits::UPPER | ASCIITraits::UPPERHEX, 'f', 'F', 15},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'g', 'G'},

    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'h', 'H'}, // 0x48
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'i', 'I'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'j', 'J'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'k', 'K'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'l', 'L'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'm', 'M'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'n', 'N'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'o', 'O'},

    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'p', 'P'}, // 0x50
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'q', 'Q'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'r', 'R'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 's', 'S'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 't', 'T'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'u', 'U'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'v', 'V'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'w', 'W'},

    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'x', 'X'}, // 0x58
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'y', 'Y'},
    {ASCIITraits::ASCII | ASCIITraits::UPPER, 'z', 'Z'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '[', '['}, // left square bracket
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '\\', '\\'}, // backslash
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, ']', ']'}, // right square bracket
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, 0x5e, 0x5e},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, 0x5f, 0x5f},

    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, 0x60, 0x60}, // 0x60
    {ASCIITraits::ASCII | ASCIITraits::LOWER | ASCIITraits::LOWERHEX, 'a', 'A', 10},
    {ASCIITraits::ASCII | ASCIITraits::LOWER | ASCIITraits::LOWERHEX, 'b', 'B', 11},
    {ASCIITraits::ASCII | ASCIITraits::LOWER | ASCIITraits::LOWERHEX, 'c', 'C', 12},
    {ASCIITraits::ASCII | ASCIITraits::LOWER | ASCIITraits::LOWERHEX, 'd', 'D', 13},
    {ASCIITraits::ASCII | ASCIITraits::LOWER | ASCIITraits::LOWERHEX, 'e', 'E', 14},
    {ASCIITraits::ASCII | ASCIITraits::LOWER | ASCIITraits::LOWERHEX, 'f', 'F', 15},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'g', 'G'},

    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'h', 'H'}, // 0x68
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'i', 'I'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'j', 'J'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'k', 'K'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'l', 'L'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'm', 'M'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'n', 'N'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'o', 'O'},

    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'p', 'P'}, // 0x70
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'q', 'Q'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'r', 'R'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 's', 'S'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 't', 'T'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'u', 'U'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'v', 'V'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'w', 'W'},

    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'x', 'X'}, // 0x78
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'y', 'Y'},
    {ASCIITraits::ASCII | ASCIITraits::LOWER, 'z', 'Z'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '{', '{'}, // 0x7b - left curly bracket
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '|', '|'},
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '}', '}'}, // 0x7d - right curly bracket
    {ASCIITraits::ASCII | ASCIITraits::PUNCTUATION, '~', '~'}, // tilde
    {ASCIITraits::ASCII | ASCIITraits::CONTROL, 0x7f, 0x7f}, // DEL

    {0x00, 0x80, 0x80}, {0x00, 0x81, 0x81}, {0x00, 0x82, 0x82}, {0x00, 0x83, 0x83},
    {0x00, 0x84, 0x84}, {0x00, 0x85, 0x85}, {0x00, 0x86, 0x86}, {0x00, 0x87, 0x87},
    {0x00, 0x88, 0x88}, {0x00, 0x89, 0x89}, {0x00, 0x8a, 0x8a}, {0x00, 0x8b, 0x8b},
    {0x00, 0x8c, 0x8c}, {0x00, 0x8d, 0x8d}, {0x00, 0x8e, 0x8e}, {0x00, 0x8f, 0x8f},

    {0x00, 0x90, 0x90}, {0x00, 0x91, 0x91}, {0x00, 0x92, 0x92}, {0x00, 0x93, 0x93},
    {0x00, 0x94, 0x94}, {0x00, 0x95, 0x95}, {0x00, 0x96, 0x96}, {0x00, 0x97, 0x97},
    {0x00, 0x98, 0x98}, {0x00, 0x99, 0x99}, {0x00, 0x9a, 0x9a}, {0x00, 0x9b, 0x9b},
    {0x00, 0x9c, 0x9c}, {0x00, 0x9d, 0x9d}, {0x00, 0x9e, 0x9e}, {0x00, 0x9f, 0x9f},

    {0x00, 0xa0, 0xa0}, {0x00, 0xa1, 0xa1}, {0x00, 0xa2, 0xa2}, {0x00, 0xa3, 0xa3},
    {0x00, 0xa4, 0xa4}, {0x00, 0xa5, 0xa5}, {0x00, 0xa6, 0xa6}, {0x00, 0xa7, 0xa7},
    {0x00, 0xa8, 0xa8}, {0x00, 0xa9, 0xa9}, {0x00, 0xaa, 0xaa}, {0x00, 0xab, 0xab},
    {0x00, 0xac, 0xac}, {0x00, 0xad, 0xad}, {0x00, 0xae, 0xae}, {0x00, 0xaf, 0xaf},

    {0x00, 0xb0, 0xb0}, {0x00, 0xb1, 0xb1}, {0x00, 0xb2, 0xb2}, {0x00, 0xb3, 0xb3},
    {0x00, 0xb4, 0xb4}, {0x00, 0xb5, 0xb5}, {0x00, 0xb6, 0xb6}, {0x00, 0xb7, 0xb7},
    {0x00, 0xb8, 0xb8}, {0x00, 0xb9, 0xb9}, {0x00, 0xba, 0xba}, {0x00, 0xbb, 0xbb},
    {0x00, 0xbc, 0xbc}, {0x00, 0xbd, 0xbd}, {0x00, 0xbe, 0xbe}, {0x00, 0xbf, 0xbf},

    {0x00, 0xc0, 0xc0}, {0x00, 0xc1, 0xc1}, {0x00, 0xc2, 0xc2}, {0x00, 0xc3, 0xc3},
    {0x00, 0xc4, 0xc4}, {0x00, 0xc5, 0xc5}, {0x00, 0xc6, 0xc6}, {0x00, 0xc7, 0xc7},
    {0x00, 0xc8, 0xc8}, {0x00, 0xc9, 0xc9}, {0x00, 0xca, 0xca}, {0x00, 0xcb, 0xcb},
    {0x00, 0xcc, 0xcc}, {0x00, 0xcd, 0xcd}, {0x00, 0xce, 0xce}, {0x00, 0xcf, 0xcf},

    {0x00, 0xd0, 0xd0}, {0x00, 0xd1, 0xd1}, {0x00, 0xd2, 0xd2}, {0x00, 0xd3, 0xd3},
    {0x00, 0xd4, 0xd4}, {0x00, 0xd5, 0xd5}, {0x00, 0xd6, 0xd6}, {0x00, 0xd7, 0xd7},
    {0x00, 0xd8, 0xd8}, {0x00, 0xd9, 0xd9}, {0x00, 0xda, 0xda}, {0x00, 0xdb, 0xdb},
    {0x00, 0xdc, 0xdc}, {0x00, 0xdd, 0xdd}, {0x00, 0xde, 0xde}, {0x00, 0xdf, 0xdf},

    {0x00, 0xe0, 0xe0}, {0x00, 0xe1, 0xe1}, {0x00, 0xe2, 0xe2}, {0x00, 0xe3, 0xe3},
    {0x00, 0xe4, 0xe4}, {0x00, 0xe5, 0xe5}, {0x00, 0xe6, 0xe6}, {0x00, 0xe7, 0xe7},
    {0x00, 0xe8, 0xe8}, {0x00, 0xe9, 0xe9}, {0x00, 0xea, 0xea}, {0x00, 0xeb, 0xeb},
    {0x00, 0xec, 0xec}, {0x00, 0xed, 0xed}, {0x00, 0xee, 0xee}, {0x00, 0xef, 0xef},

    {0x00, 0xf0, 0xf0}, {0x00, 0xf1, 0xf1}, {0x00, 0xf2, 0xf2}, {0x00, 0xf3, 0xf3},
    {0x00, 0xf4, 0xf4}, {0x00, 0xf5, 0xf5}, {0x00, 0xf6, 0xf6}, {0x00, 0xf7, 0xf7},
    {0x00, 0xf8, 0xf8}, {0x00, 0xf9, 0xf9}, {0x00, 0xfa, 0xfa}, {0x00, 0xfb, 0xfb},
    {0x00, 0xfc, 0xfc}, {0x00, 0xfd, 0xfd}, {0x00, 0xfe, 0xfe}, {0x00, 0xff, 0xff}
  };
  
};

const ASCIITraits::CharacterDescriptor* ASCIITraits::lookup = ASCIITraitsImpl::lookup;

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
