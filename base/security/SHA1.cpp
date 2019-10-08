/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/SHA1.h>
#include <base/Functor.h>
#include <base/security/Base64.h>
#include <base/string/ASCIITraits.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

SHA1::SHA1() throw() {
  messageDigest[0] = 0x67452301; // H0
  messageDigest[1] = 0xefcdab89; // H1
  messageDigest[2] = 0x98badcfe; // H2
  messageDigest[3] = 0x10325476; // H3
  messageDigest[4] = 0xc3d2e1f0; // H4
}

void SHA1::pushBlock(const uint8* block) throw() {
  uint32 a = messageDigest[0]; // A = H0
  uint32 b = messageDigest[1]; // B = H1
  uint32 c = messageDigest[2]; // C = H2
  uint32 d = messageDigest[3]; // D = H3
  uint32 e = messageDigest[4]; // E = H4

  uint32 words[MESSAGE_SCHEDULE];
  for (unsigned int t = 0; t < 16; ++t) { // Copy M into W0 to W15
    words[t] = ByteIO::readUInt32(block);
    block += 4;
  }
  for (unsigned int t = 16; t < MESSAGE_SCHEDULE; ++t) {
    words[t] = rotate(words[t-3] ^ words[t-8] ^ words[t-14] ^ words[t-16], 1);
  }

  translate1(a, b, c, d, e, words[0]);
  translate1(e, a, b, c, d, words[1]);
  translate1(d, e, a, b, c, words[2]);
  translate1(c, d, e, a, b, words[3]);
  translate1(b, c, d, e, a, words[4]);
  translate1(a, b, c, d, e, words[5]);
  translate1(e, a, b, c, d, words[6]);
  translate1(d, e, a, b, c, words[7]);
  translate1(c, d, e, a, b, words[8]);
  translate1(b, c, d, e, a, words[9]);
  translate1(a, b, c, d, e, words[10]);
  translate1(e, a, b, c, d, words[11]);
  translate1(d, e, a, b, c, words[12]);
  translate1(c, d, e, a, b, words[13]);
  translate1(b, c, d, e, a, words[14]);
  translate1(a, b, c, d, e, words[15]);
  translate1(e, a, b, c, d, words[16]);
  translate1(d, e, a, b, c, words[17]);
  translate1(c, d, e, a, b, words[18]);
  translate1(b, c, d, e, a, words[19]);

  translate2(a, b, c, d, e, words[20]);
  translate2(e, a, b, c, d, words[21]);
  translate2(d, e, a, b, c, words[22]);
  translate2(c, d, e, a, b, words[23]);
  translate2(b, c, d, e, a, words[24]);
  translate2(a, b, c, d, e, words[25]);
  translate2(e, a, b, c, d, words[26]);
  translate2(d, e, a, b, c, words[27]);
  translate2(c, d, e, a, b, words[28]);
  translate2(b, c, d, e, a, words[29]);
  translate2(a, b, c, d, e, words[30]);
  translate2(e, a, b, c, d, words[31]);
  translate2(d, e, a, b, c, words[32]);
  translate2(c, d, e, a, b, words[33]);
  translate2(b, c, d, e, a, words[34]);
  translate2(a, b, c, d, e, words[35]);
  translate2(e, a, b, c, d, words[36]);
  translate2(d, e, a, b, c, words[37]);
  translate2(c, d, e, a, b, words[38]);
  translate2(b, c, d, e, a, words[39]);

  translate3(a, b, c, d, e, words[40]);
  translate3(e, a, b, c, d, words[41]);
  translate3(d, e, a, b, c, words[42]);
  translate3(c, d, e, a, b, words[43]);
  translate3(b, c, d, e, a, words[44]);
  translate3(a, b, c, d, e, words[45]);
  translate3(e, a, b, c, d, words[46]);
  translate3(d, e, a, b, c, words[47]);
  translate3(c, d, e, a, b, words[48]);
  translate3(b, c, d, e, a, words[49]);
  translate3(a, b, c, d, e, words[50]);
  translate3(e, a, b, c, d, words[51]);
  translate3(d, e, a, b, c, words[52]);
  translate3(c, d, e, a, b, words[53]);
  translate3(b, c, d, e, a, words[54]);
  translate3(a, b, c, d, e, words[55]);
  translate3(e, a, b, c, d, words[56]);
  translate3(d, e, a, b, c, words[57]);
  translate3(c, d, e, a, b, words[58]);
  translate3(b, c, d, e, a, words[59]);

  translate4(a, b, c, d, e, words[60]);
  translate4(e, a, b, c, d, words[61]);
  translate4(d, e, a, b, c, words[62]);
  translate4(c, d, e, a, b, words[63]);
  translate4(b, c, d, e, a, words[64]);
  translate4(a, b, c, d, e, words[65]);
  translate4(e, a, b, c, d, words[66]);
  translate4(d, e, a, b, c, words[67]);
  translate4(c, d, e, a, b, words[68]);
  translate4(b, c, d, e, a, words[69]);
  translate4(a, b, c, d, e, words[70]);
  translate4(e, a, b, c, d, words[71]);
  translate4(d, e, a, b, c, words[72]);
  translate4(c, d, e, a, b, words[73]);
  translate4(b, c, d, e, a, words[74]);
  translate4(a, b, c, d, e, words[75]);
  translate4(e, a, b, c, d, words[76]);
  translate4(d, e, a, b, c, words[77]);
  translate4(c, d, e, a, b, words[78]);
  translate4(b, c, d, e, a, words[79]);

  messageDigest[0] += a;
  messageDigest[1] += b;
  messageDigest[2] += c;
  messageDigest[3] += d;
  messageDigest[4] += e;
}

unsigned int SHA1::push(const uint8* buffer, unsigned int size) throw(OutOfRange) {
  bassert(size < MAXIMUM_SIZE - totalSize, OutOfRange());
  unsigned int result = size;
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
  return result;
}

void SHA1::pushEnd() throw() {
  ASSERT(bytesInBuffer < BLOCK_SIZE);
  buffer[bytesInBuffer++] = 0x80; // append 0b10000000
  
  const unsigned int SIZE_FIELD = 64/8;
  
  if (bytesInBuffer >= (BLOCK_SIZE - SIZE_FIELD)) { // do we need one more block
    while (bytesInBuffer < BLOCK_SIZE) { // zero padding
      buffer[bytesInBuffer++] = 0x00;
    }
    pushBlock(buffer);
    bytesInBuffer = 0;
  }
  while (bytesInBuffer < (BLOCK_SIZE - SIZE_FIELD)) { // zero padding
    buffer[bytesInBuffer++] = 0x00;
  }

  // append length of original message to block
  uint64 temp = totalSize * 8;
  for (unsigned int i = sizeof(temp); i > 0;) {
    --i;
    buffer[bytesInBuffer++] = temp >> (i << 3);
  }
  pushBlock(buffer);
  bytesInBuffer = 0;
}

String SHA1::getValue() const throw() { 
  String result(sizeof(messageDigest) * 2);
  result.forceToLength(sizeof(messageDigest) * 2);
  String::Iterator i = result.getBeginIterator();
  for (unsigned int j = 0; j < getArraySize(messageDigest); ++j) {
    uint32 word = messageDigest[j];
    for (unsigned int k = sizeof(uint32) * 8; k > 0;) {
      k -= 4;
      *i++ = ASCIITraits::valueToDigit((word >> k) & 0x0f); // high-order digit
      k -= 4;
      *i++ = ASCIITraits::valueToDigit((word >> k) & 0x0f); // low-order digit
    }
  }
  return result;
}

String SHA1::getBase64() const throw() {
  uint8 temp[sizeof(messageDigest)];
  uint8* p = temp;
  for (unsigned int j = 0; j < getArraySize(messageDigest); ++j) {
    uint32 word = messageDigest[j];
    for (unsigned int k = 0; k < sizeof(uint32); ++k, word >>= 8) {
      *p++ = word;
    }
  }
  return Base64::encode(temp, sizeof(temp));
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
