/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__HASH_H
#define _DK_SDU_MIP__BASE_COLLECTION__HASH_H

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Hash operator.
  
  @short Hash
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Hash {
public:

  inline unsigned long operator()(const TYPE& value) throw() {
    return 0;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif

#if 0
unsigned int sdbm(const char* buffer, unsigned int size) throw() {
  unsigned int result = 0;
  const char* src = buffer;
  const char* end = buffer + size;
  while (src < end) {
    result = 65599 * result + static_cast<unsigned char>(*src++);
  }
  return hash;
}

// see http://burtleburtle.net/bob/c/lookup8.c
// must add comment if used

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}


uint32 bobHash(const char* key, unsigned int size, uin32 initial) {
  unsigned int len = size;
  uint32 a = b = 0x9e3779b9; /* the golden ratio; an arbitrary value */
  uint32 c = initial; // initial hash

  const char* end = key + (size/12)*12;

  // cast and byte order must be fixed
  while (key < end) {
    a += *key++ +((uint32)*key++ << 8) + ((uint32)*key++ << 16) + ((uint32)*key++ << 24);
    b += *key++ +((uint32)*key++ << 8) + ((uint32)*key++ << 16) + ((uint32)*key++ << 24);
    c += *key++ +((uint32)*key++ << 8) + ((uint32)*key++ << 16) + ((uint32)*key++ << 24);
    mix(a,b,c);
  }
  
  // handle the last bytes   
  c += size;
  switch (size % 12) {
  case 11: c += ((uint32)key[10] << 24);
  case 10: c += ((uint32)key[9] << 16);
  case 9 : c += ((uint32)key[8] << 8);
  case 8 : b += ((uint32)key[7] << 24);
  case 7 : b += ((uint32)key[6] << 16);
  case 6 : b += ((uint32)key[5] << 8);
  case 5 : b += key[4];
  case 4 : a += ((uint32)key[3] << 24);
  case 3 : a += ((uint32)key[2] << 16);
  case 2 : a += ((uint32)key[1] << 8);
  case 1 : a += key[0];
  }
  mix(a,b,c);
  return c;
}


inline uint64 mix64(uint64 a, uint64 b, uint64 c) throw() {
  a = a - b;
  a = a - c;
  a = a^(c >> 43);
  b = b - c;
  b = b - a;
  b = b^(a << 9);
  c = c - a;
  c = c - b;
  c = c^(b >> 8);
  a = a - b;
  a = a - c;
  a = a^(c >> 38);
  b = b - c;
  b = b - a;
  b = b^(a << 23);
  c = c - a;
  c = c - b;
  c = c^(b >> 5);
  a = a - b;
  a = a - c;
  a = a^(c >> 35);
  b = b - c;
  b = b - a;
  b = b^(a << 49);
  c = c - a;
  c = c - b;
  c = c^(b >> 11);
  a = a - b;
  a = a - c;
  a = a^(c >> 12);
  b = b - c;
  b = b - a;
  b = b^(a << 18);
  c = c - a;
  c = c - b;
  c = c^(b >> 22);
}

The 64-bit golden ratio is 0x9e3779b97f4a7c13LL. 
#endif
