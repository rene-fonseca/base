/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/MD5Sum.h>
#include <base/Functor.h>
#include <base/security/Base64.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class MD5SumImpl {
public:

  static inline unsigned int F(unsigned int X, unsigned int Y, unsigned int Z) throw() {
    return X & Y | ~X & Z;
  }

  static inline unsigned int G(unsigned int X, unsigned int Y, unsigned int Z) throw() {
    return X & Z | Y & ~Z;
  }

  static inline unsigned int H(unsigned int X, unsigned int Y, unsigned int Z) throw() {
    return X ^ Y ^ Z;
  }

  static inline unsigned int I(unsigned int X, unsigned int Y, unsigned int Z) throw() {
    return Y ^ (X | ~Z);
  }

  typedef unsigned int (*Operation)(unsigned int, unsigned int, unsigned int);

  static inline unsigned int rotate(unsigned int value, unsigned int bits) throw() {
    return (value << bits) | (value >> (32 - bits));
  }

  static inline void translate(Operation opr, unsigned int& a, unsigned int b, unsigned int c, unsigned int d, unsigned int k, unsigned int s, unsigned int T) throw() {
    a = b + rotate(a + opr(b, c, d) + k + T, s);
  }
};

MD5Sum::MD5Sum() throw() : totalSize(0), bytesInBuffer(0) {
  messageDigest[0] = 0x67452301;
  messageDigest[1] = 0xefcdab89;
  messageDigest[2] = 0x98badcfe;
  messageDigest[3] = 0x10325476;
}

void MD5Sum::pushBlock(const byte* block) throw() {
  unsigned int a = messageDigest[0];
  unsigned int b = messageDigest[1];
  unsigned int c = messageDigest[2];
  unsigned int d = messageDigest[3];

  unsigned int words[16];
  --block;
  for (unsigned int i = 0; i < 16; ++i) {
    words[i] = (*++block << 0) | (*++block << 8) | (*++block << 16) | (*++block << 24);
  }

  // round 1
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[0], 7, 0xd76aa478);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[1], 12, 0xe8c7b756);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[2], 17, 0x242070db);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[3], 22, 0xc1bdceee);
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[4], 7, 0xf57c0faf);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[5], 12, 0x4787c62a);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[6], 17, 0xa8304613);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[7], 22, 0xfd469501);
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[8], 7, 0x698098d8);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[9], 12, 0x8b44f7af);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[10], 17, 0xffff5bb1);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[11], 22, 0x895cd7be);
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[12], 7, 0x6b901122);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[13], 12, 0xfd987193);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[14], 17, 0xa679438e);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[15], 22, 0x49b40821);

  // round 2
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[1], 5, 0xf61e2562);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[6], 9, 0xc040b340);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[11], 14, 0x265e5a51);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[0], 20, 0xe9b6c7aa);
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[5], 5, 0xd62f105d);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[10], 9, 0x02441453);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[15], 14, 0xd8a1e681);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[4], 20, 0xe7d3fbc8);
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[9], 5, 0x21e1cde6);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[14], 9, 0xc33707d6);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[3], 14, 0xf4d50d87);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[8], 20, 0x455a14ed);
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[13], 5, 0xa9e3e905);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[2], 9, 0xfcefa3f8);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[7], 14, 0x676f02d9);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[12], 20, 0x8d2a4c8a);

  // round 3
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[5], 4, 0xfffa3942);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[8], 11, 0x8771f681);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[11], 16, 0x6d9d6122);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[14], 23, 0xfde5380c);
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[1], 4, 0xa4beea44);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[4], 11, 0x4bdecfa9);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[7], 16, 0xf6bb4b60);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[10], 23, 0xbebfbc70);
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[13], 4, 0x289b7ec6);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[0], 11, 0xeaa127fa);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[3], 16, 0xd4ef3085);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[6], 23, 0x04881d05);
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[9], 4, 0xd9d4d039);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[12], 11, 0xe6db99e5);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[15], 16, 0x1fa27cf8);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[2], 23, 0xc4ac5665);

  // round 4
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[0], 6, 0xf4292244);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[7], 10, 0x432aff97);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[14], 15, 0xab9423a7);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[5], 21, 0xfc93a039);
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[12], 6, 0x655b59c3);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[3], 10, 0x8f0ccc92);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[10], 15, 0xffeff47d);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[1], 21, 0x85845dd1);
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[8], 6, 0x6fa87e4f);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[15], 10, 0xfe2ce6e0);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[6], 15, 0xa3014314);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[13], 21, 0x4e0811a1);
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[4], 6, 0xf7537e82);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[11], 10, 0xbd3af235);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[2], 15, 0x2ad7d2bb);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[9], 21, 0xeb86d391);

  messageDigest[0] += a;
  messageDigest[1] += b;
  messageDigest[2] += c;
  messageDigest[3] += d;
}

void MD5Sum::push(const byte* buffer, unsigned int size) throw(OutOfRange) {
  assert(size < MAXIMUM_SIZE - totalSize, OutOfRange());
  totalSize += size;
  if (size + bytesInBuffer >= BLOCK_SIZE) { // do we have a complete block
    if (bytesInBuffer > 0) { // do we need to empty internal buffer
      copy(this->buffer + bytesInBuffer, buffer, BLOCK_SIZE - bytesInBuffer);
      pushBlock(this->buffer); // internal buffer
      buffer += BLOCK_SIZE - bytesInBuffer;
      size -= BLOCK_SIZE - bytesInBuffer;
      bytesInBuffer = 0;
    }
    while (size >= BLOCK_SIZE) {
      pushBlock(buffer); // external buffer
      buffer += BLOCK_SIZE;
      size -= BLOCK_SIZE;
    }
  }

  copy(this->buffer + bytesInBuffer, buffer, size); // put remaining octets into internal buffer
  bytesInBuffer += size;
}

void MD5Sum::pushEnd() throw() {
  ASSERT(bytesInBuffer < BLOCK_SIZE);
  buffer[bytesInBuffer++] = 0x80; // append 0b10000000

  if (bytesInBuffer >= BLOCK_SIZE - 2 * 4) { // do we need one more block
    while (bytesInBuffer < BLOCK_SIZE) { // zero padding
      buffer[bytesInBuffer++] = 0x00;
    }
    pushBlock(buffer);
    bytesInBuffer = 0;
  }
  while (bytesInBuffer < BLOCK_SIZE - 2 * 4) { // zero padding
    buffer[bytesInBuffer++] = 0x00;
  }

  // append length of original message to block
  unsigned long long temp = totalSize * 8;
  for (unsigned int i = 0; i < 64; i += 8) {
    buffer[bytesInBuffer++] = temp >> i;
  }
  pushBlock(buffer);
  bytesInBuffer = 0;
}

unsigned long long MD5Sum::getTotalSize() const throw() {
  return totalSize;
}

String MD5Sum::getValue() const throw() {
  String result(4 * 4 * 2);
  result.forceToLength(4 * 4 * 2);
  String::Iterator i = result.getBeginIterator();
  for (unsigned int j = 0; j < 4; ++j) {
    unsigned int word = messageDigest[j];
    for (unsigned int k = 0; k < 4; ++k, word >>= 8) {
      *i++ = ASCIITraits::valueToDigit((word >> 4) & 0x0f); // high-order digit
      *i++ = ASCIITraits::valueToDigit(word & 0x0f); // low-order digit
    }
  }
  return result;
}

String MD5Sum::getBase64() const throw() {
  byte temp[16];
  byte* p = &temp[0];
  for (unsigned int j = 0; j < 4; ++j) {
    unsigned int word = messageDigest[j];
    for (unsigned int k = 0; k < 4; ++k, word >>= 8) {
      *p++ = word & 0xff;
    }
  }
  return Base64::encode(&temp[0], 16);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
