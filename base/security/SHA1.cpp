/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/SHA1.h>
#include <base/Functor.h>
#include <base/security/Base64.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class SHA1Impl {
public:

  static inline unsigned int rotate(unsigned int value, unsigned int bits) throw() {
    return (value << bits) | (value >> (32 - bits));
  }

  static inline void translate1(unsigned int a, unsigned int& b, unsigned int c, unsigned int d, unsigned int& e, unsigned int word) throw() {
    e += rotate(a, 5) + ((b & c) | (~b & d)) + word + 0x5a827999;
    b = rotate(b, 30);
  }

  static inline void translate2(unsigned int a, unsigned int& b, unsigned int c, unsigned int d, unsigned int& e, unsigned int word) throw() {
    e += rotate(a, 5) + (b ^ c ^ d) + word + 0x6ed9eba1;
    b = rotate(b, 30);
  }

  static inline void translate3(unsigned int a, unsigned int& b, unsigned int c, unsigned int d, unsigned int& e, unsigned int word) throw() {
    e += rotate(a, 5) + ((b & c) | (b & d) | (c & d)) + word + 0x8f1bbcdc;
    b = rotate(b, 30);
  }

  static inline void translate4(unsigned int a, unsigned int& b, unsigned int c, unsigned int d, unsigned int& e, unsigned int word) throw() {
    e += rotate(a, 5) + (b ^ c ^ d) + word + 0xca62c1d6;
    b = rotate(b, 30);
  }
};

SHA1::SHA1() throw() : totalSize(0), bytesInBuffer(0) {
  messageDigest[0] = 0x67452301; // H0
  messageDigest[1] = 0xefcdab89; // H1
  messageDigest[2] = 0x98badcfe; // H2
  messageDigest[3] = 0x10325476; // H3
  messageDigest[4] = 0xc3d2e1f0; // H4
}

void SHA1::pushBlock(const byte* block) throw() {
  unsigned int a = messageDigest[0]; // A = H0
  unsigned int b = messageDigest[1]; // B = H1
  unsigned int c = messageDigest[2]; // C = H2
  unsigned int d = messageDigest[3]; // D = H3
  unsigned int e = messageDigest[4]; // E = H4

  unsigned int words[80];
  --block;
  for (unsigned int t = 0; t < 16; ++t) { // Copy M into W0 to W15
    words[t] = (*++block << 24) | (*++block << 16) | (*++block << 8) | (*++block << 0);
  }
  for (unsigned int t = 16; t < 80; ++t) {
    words[t] = SHA1Impl::rotate(words[t-3] ^ words[t-8] ^ words[t-14] ^ words[t-16], 1);
  }

  SHA1Impl::translate1(a, b, c, d, e, words[0]);
  SHA1Impl::translate1(e, a, b, c, d, words[1]);
  SHA1Impl::translate1(d, e, a, b, c, words[2]);
  SHA1Impl::translate1(c, d, e, a, b, words[3]);
  SHA1Impl::translate1(b, c, d, e, a, words[4]);
  SHA1Impl::translate1(a, b, c, d, e, words[5]);
  SHA1Impl::translate1(e, a, b, c, d, words[6]);
  SHA1Impl::translate1(d, e, a, b, c, words[7]);
  SHA1Impl::translate1(c, d, e, a, b, words[8]);
  SHA1Impl::translate1(b, c, d, e, a, words[9]);
  SHA1Impl::translate1(a, b, c, d, e, words[10]);
  SHA1Impl::translate1(e, a, b, c, d, words[11]);
  SHA1Impl::translate1(d, e, a, b, c, words[12]);
  SHA1Impl::translate1(c, d, e, a, b, words[13]);
  SHA1Impl::translate1(b, c, d, e, a, words[14]);
  SHA1Impl::translate1(a, b, c, d, e, words[15]);
  SHA1Impl::translate1(e, a, b, c, d, words[16]);
  SHA1Impl::translate1(d, e, a, b, c, words[17]);
  SHA1Impl::translate1(c, d, e, a, b, words[18]);
  SHA1Impl::translate1(b, c, d, e, a, words[19]);

  SHA1Impl::translate2(a, b, c, d, e, words[20]);
  SHA1Impl::translate2(e, a, b, c, d, words[21]);
  SHA1Impl::translate2(d, e, a, b, c, words[22]);
  SHA1Impl::translate2(c, d, e, a, b, words[23]);
  SHA1Impl::translate2(b, c, d, e, a, words[24]);
  SHA1Impl::translate2(a, b, c, d, e, words[25]);
  SHA1Impl::translate2(e, a, b, c, d, words[26]);
  SHA1Impl::translate2(d, e, a, b, c, words[27]);
  SHA1Impl::translate2(c, d, e, a, b, words[28]);
  SHA1Impl::translate2(b, c, d, e, a, words[29]);
  SHA1Impl::translate2(a, b, c, d, e, words[30]);
  SHA1Impl::translate2(e, a, b, c, d, words[31]);
  SHA1Impl::translate2(d, e, a, b, c, words[32]);
  SHA1Impl::translate2(c, d, e, a, b, words[33]);
  SHA1Impl::translate2(b, c, d, e, a, words[34]);
  SHA1Impl::translate2(a, b, c, d, e, words[35]);
  SHA1Impl::translate2(e, a, b, c, d, words[36]);
  SHA1Impl::translate2(d, e, a, b, c, words[37]);
  SHA1Impl::translate2(c, d, e, a, b, words[38]);
  SHA1Impl::translate2(b, c, d, e, a, words[39]);

  SHA1Impl::translate3(a, b, c, d, e, words[40]);
  SHA1Impl::translate3(e, a, b, c, d, words[41]);
  SHA1Impl::translate3(d, e, a, b, c, words[42]);
  SHA1Impl::translate3(c, d, e, a, b, words[43]);
  SHA1Impl::translate3(b, c, d, e, a, words[44]);
  SHA1Impl::translate3(a, b, c, d, e, words[45]);
  SHA1Impl::translate3(e, a, b, c, d, words[46]);
  SHA1Impl::translate3(d, e, a, b, c, words[47]);
  SHA1Impl::translate3(c, d, e, a, b, words[48]);
  SHA1Impl::translate3(b, c, d, e, a, words[49]);
  SHA1Impl::translate3(a, b, c, d, e, words[50]);
  SHA1Impl::translate3(e, a, b, c, d, words[51]);
  SHA1Impl::translate3(d, e, a, b, c, words[52]);
  SHA1Impl::translate3(c, d, e, a, b, words[53]);
  SHA1Impl::translate3(b, c, d, e, a, words[54]);
  SHA1Impl::translate3(a, b, c, d, e, words[55]);
  SHA1Impl::translate3(e, a, b, c, d, words[56]);
  SHA1Impl::translate3(d, e, a, b, c, words[57]);
  SHA1Impl::translate3(c, d, e, a, b, words[58]);
  SHA1Impl::translate3(b, c, d, e, a, words[59]);

  SHA1Impl::translate4(a, b, c, d, e, words[60]);
  SHA1Impl::translate4(e, a, b, c, d, words[61]);
  SHA1Impl::translate4(d, e, a, b, c, words[62]);
  SHA1Impl::translate4(c, d, e, a, b, words[63]);
  SHA1Impl::translate4(b, c, d, e, a, words[64]);
  SHA1Impl::translate4(a, b, c, d, e, words[65]);
  SHA1Impl::translate4(e, a, b, c, d, words[66]);
  SHA1Impl::translate4(d, e, a, b, c, words[67]);
  SHA1Impl::translate4(c, d, e, a, b, words[68]);
  SHA1Impl::translate4(b, c, d, e, a, words[69]);
  SHA1Impl::translate4(a, b, c, d, e, words[70]);
  SHA1Impl::translate4(e, a, b, c, d, words[71]);
  SHA1Impl::translate4(d, e, a, b, c, words[72]);
  SHA1Impl::translate4(c, d, e, a, b, words[73]);
  SHA1Impl::translate4(b, c, d, e, a, words[74]);
  SHA1Impl::translate4(a, b, c, d, e, words[75]);
  SHA1Impl::translate4(e, a, b, c, d, words[76]);
  SHA1Impl::translate4(d, e, a, b, c, words[77]);
  SHA1Impl::translate4(c, d, e, a, b, words[78]);
  SHA1Impl::translate4(b, c, d, e, a, words[79]);

  messageDigest[0] += a;
  messageDigest[1] += b;
  messageDigest[2] += c;
  messageDigest[3] += d;
  messageDigest[4] += e;
}

void SHA1::push(const byte* buffer, unsigned int size) throw(OutOfRange) {
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

void SHA1::pushEnd() throw() {
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
  for (int i = 7; i >= 0; --i) {
    buffer[bytesInBuffer++] = temp >> (i << 3);
  }
  pushBlock(buffer);
  bytesInBuffer = 0;
}

unsigned long long SHA1::getTotalSize() const throw() {
  return totalSize;
}

String SHA1::getValue() const throw() {
  String result(5 * 4 * 2);
  result.forceToLength(5 * 4 * 2);
  String::Iterator i = result.getBeginIterator();
  for (unsigned int j = 0; j < 5; ++j) {
    unsigned int word = messageDigest[j];
    for (int k = 24; k >= 0; k -= 8) {
      *i++ = ASCIITraits::valueToDigit(((word >> k) >> 4) & 0x0f); // high-order digit
      *i++ = ASCIITraits::valueToDigit((word >> k) & 0x0f); // low-order digit
    }
  }
  return result;
}

String SHA1::getBase64() const throw() {
  byte temp[16];
  byte* p = &temp[0];
  for (unsigned int j = 0; j < 5; ++j) {
    unsigned int word = messageDigest[j];
    for (unsigned int k = 0; k < 4; ++k, word >>= 8) {
      *p++ = word & 0xff;
    }
  }
  return Base64::encode(&temp[0], 16);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
