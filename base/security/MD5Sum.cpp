/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/MD5Sum.h>
#include <base/security/Bytes.h>
#include <base/Functor.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class MD5SumImpl {
public:

  static inline uint32 F(uint32 X, uint32 Y, uint32 Z) noexcept
  {
    return (X & Y) | (~X & Z);
  }

  static inline uint32 G(uint32 X, uint32 Y, uint32 Z) noexcept
  {
    return (X & Z) | (Y & ~Z);
  }

  static inline uint32 H(uint32 X, uint32 Y, uint32 Z) noexcept
  {
    return X ^ Y ^ Z;
  }

  static inline uint32 I(uint32 X, uint32 Y, uint32 Z) noexcept
  {
    return Y ^ (X | ~Z);
  }

  typedef uint32(*Operation)(uint32, uint32, uint32);

  static inline uint32 rotate(uint32 value, uint32 bits) noexcept
  {
    return (value << bits) | (value >> (32 - bits));
  }

  static inline void translate(
    Operation opr,
    uint32& a,
    uint32 b,
    uint32 c,
    uint32 d,
    uint32 k,
    uint32 s,
    uint32 T) noexcept
  {
    a = b + rotate(a + opr(b, c, d) + k + T, s);
  }
};

MD5Sum::MD5Sum() noexcept
{
  clear(buffer);

  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;
}

void MD5Sum::pushBlock(const uint8* block) noexcept
{
  uint32 a = state[0];
  uint32 b = state[1];
  uint32 c = state[2];
  uint32 d = state[3];

  // decode
  uint32 words[BLOCK_SIZE / sizeof(uint32)];
  for (unsigned int i = 0; i < BLOCK_SIZE / sizeof(uint32); ++i) {
    words[i] = (static_cast<uint32>(block[0]) << 0) |
      (static_cast<uint32>(block[1]) << 8) |
      (static_cast<uint32>(block[2]) << 16) |
      (static_cast<uint32>(block[3]) << 24);
    block += sizeof(uint32);
  }

  enum {
    S11 = 7,
    S12 = 12,
    S13 = 17,
    S14 = 22,
    S21 = 5,
    S22 = 9,
    S23 = 14,
    S24 = 20,
    S31 = 4,
    S32 = 11,
    S33 = 16,
    S34 = 23,
    S41 = 6,
    S42 = 10,
    S43 = 15,
    S44 = 21
  };

  // round 1
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[ 0], S11, 0xd76aa478);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[ 1], S12, 0xe8c7b756);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[ 2], S13, 0x242070db);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[ 3], S14, 0xc1bdceee);
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[ 4], S11, 0xf57c0faf);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[ 5], S12, 0x4787c62a);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[ 6], S13, 0xa8304613);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[ 7], S14, 0xfd469501);
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[ 8], S11, 0x698098d8);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[ 9], S12, 0x8b44f7af);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[10], S13, 0xffff5bb1);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[11], S14, 0x895cd7be);
  MD5SumImpl::translate(MD5SumImpl::F, a, b, c, d, words[12], S11, 0x6b901122);
  MD5SumImpl::translate(MD5SumImpl::F, d, a, b, c, words[13], S12, 0xfd987193);
  MD5SumImpl::translate(MD5SumImpl::F, c, d, a, b, words[14], S13, 0xa679438e);
  MD5SumImpl::translate(MD5SumImpl::F, b, c, d, a, words[15], S14, 0x49b40821);

  // round 2
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[ 1], S21, 0xf61e2562);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[ 6], S22, 0xc040b340);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[11], S23, 0x265e5a51);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[ 0], S24, 0xe9b6c7aa);
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[ 5], S21, 0xd62f105d);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[10], S22, 0x02441453);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[15], S23, 0xd8a1e681);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[ 4], S24, 0xe7d3fbc8);
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[ 9], S21, 0x21e1cde6);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[14], S22, 0xc33707d6);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[ 3], S23, 0xf4d50d87);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[ 8], S24, 0x455a14ed);
  MD5SumImpl::translate(MD5SumImpl::G, a, b, c, d, words[13], S21, 0xa9e3e905);
  MD5SumImpl::translate(MD5SumImpl::G, d, a, b, c, words[ 2], S22, 0xfcefa3f8);
  MD5SumImpl::translate(MD5SumImpl::G, c, d, a, b, words[ 7], S23, 0x676f02d9);
  MD5SumImpl::translate(MD5SumImpl::G, b, c, d, a, words[12], S24, 0x8d2a4c8a);

  // round 3
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[ 5], S31, 0xfffa3942);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[ 8], S32, 0x8771f681);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[11], S33, 0x6d9d6122);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[14], S34, 0xfde5380c);
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[ 1], S31, 0xa4beea44);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[ 4], S32, 0x4bdecfa9);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[ 7], S33, 0xf6bb4b60);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[10], S34, 0xbebfbc70);
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[13], S31, 0x289b7ec6);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[ 0], S32, 0xeaa127fa);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[ 3], S33, 0xd4ef3085);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[ 6], S34, 0x04881d05);
  MD5SumImpl::translate(MD5SumImpl::H, a, b, c, d, words[ 9], S31, 0xd9d4d039);
  MD5SumImpl::translate(MD5SumImpl::H, d, a, b, c, words[12], S32, 0xe6db99e5);
  MD5SumImpl::translate(MD5SumImpl::H, c, d, a, b, words[15], S33, 0x1fa27cf8);
  MD5SumImpl::translate(MD5SumImpl::H, b, c, d, a, words[ 2], S34, 0xc4ac5665);

  // round 4
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[ 0], S41, 0xf4292244);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[ 7], S42, 0x432aff97);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[14], S43, 0xab9423a7);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[ 5], S44, 0xfc93a039);
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[12], S41, 0x655b59c3);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[ 3], S42, 0x8f0ccc92);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[10], S43, 0xffeff47d);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[ 1], S44, 0x85845dd1);
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[ 8], S41, 0x6fa87e4f);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[15], S42, 0xfe2ce6e0);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[ 6], S43, 0xa3014314);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[13], S44, 0x4e0811a1);
  MD5SumImpl::translate(MD5SumImpl::I, a, b, c, d, words[ 4], S41, 0xf7537e82);
  MD5SumImpl::translate(MD5SumImpl::I, d, a, b, c, words[11], S42, 0xbd3af235);
  MD5SumImpl::translate(MD5SumImpl::I, c, d, a, b, words[ 2], S43, 0x2ad7d2bb);
  MD5SumImpl::translate(MD5SumImpl::I, b, c, d, a, words[ 9], S44, 0xeb86d391);

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
}

unsigned int MD5Sum::push(const uint8* buffer, unsigned int size)
{
  if (size == 0) {
    return 0;
  }
  if (closed) {
    throw OutOfRange("MD5Sum has been closed.");
  }

  bassert(size < (MAXIMUM_SIZE - totalSize), OutOfRange());
  const unsigned int result = size;
  totalSize += size;
  if ((size + bytesInBuffer) >= BLOCK_SIZE) { // do we have a complete block
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
    BASSERT(bytesInBuffer == 0);
  }

  copy(this->buffer + bytesInBuffer, buffer, size); // put remaining octets into internal buffer
  bytesInBuffer += size;
  return result;
}

void MD5Sum::pushEnd()
{
  if (closed) {
    throw OutOfRange("MD5Sum has been closed.");
  }
  closed = true;

  if (!INLINE_ASSERT(bytesInBuffer < BLOCK_SIZE)) { // not required for here for safety
    pushBlock(buffer);
    bytesInBuffer = 0;
  }

  const uint64 length = static_cast<uint64>(totalSize * 8); // modulo 2^64
  buffer[bytesInBuffer++] = 0x80; // ATTENTION: padding starts with 0x80 - minimum 1 byte padding!

  const unsigned int BIT_BYTES = 2 * 4;
  BASSERT(sizeof(length) == BIT_BYTES);

  if (bytesInBuffer > (BLOCK_SIZE - BIT_BYTES)) { // do we need one more block
    while (bytesInBuffer < BLOCK_SIZE) { // zero padding
      buffer[bytesInBuffer++] = 0x00;
    }
    pushBlock(buffer);
    bytesInBuffer = 0;
  }

  // pad to byte 56 so there is room for bit length
  while (bytesInBuffer < (BLOCK_SIZE - BIT_BYTES)) { // zero padding
    buffer[bytesInBuffer++] = 0x00;
  }

  // append length of original message to block - padding excluded
  for (unsigned int i = 0; i < (8 * BIT_BYTES); i += 8) {
    buffer[bytesInBuffer++] = static_cast<uint8>((length >> i) & 0xff);
  }
  BASSERT(bytesInBuffer == BLOCK_SIZE);
  pushBlock(buffer);
  bytesInBuffer = 0;
}

String MD5Sum::getValue() const noexcept
{
  uint8 temp[sizeof(uint32) * 4 /*getArraySize(state)*/];
  Bytes::convertWordsToBytes(temp, state, getArraySize(state));
  return Bytes::getAsHex(temp, sizeof(temp));
}

String MD5Sum::getBase64() const noexcept
{
  uint8 temp[sizeof(uint32) * 4 /*getArraySize(state)*/];
  Bytes::convertWordsToBytes(temp, state, getArraySize(state));
  return Bytes::getAsBase64(temp, sizeof(temp));
}

MD5Sum::~MD5Sum()
{
  // remove data from stack/heap
  clear(state);
  totalSize = 0;
  clear(buffer);
  bytesInBuffer = 0;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(MD5Sum) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base/security");
  TEST_IMPACT(SECURITY);

  String getMD5Sum(const uint8* buffer, MemorySize size)
  {
    MD5Sum digest;
    digest.push(buffer, size);
    digest.pushEnd();
    return digest.getValue();
  }

  String getMD5Sum(const char* text)
  {
    return getMD5Sum(reinterpret_cast<const uint8*>(text), getNullTerminatedLength(text));
  }

  void run() override
  {
    TEST_EQUAL(getMD5Sum(""), "d41d8cd98f00b204e9800998ecf8427e");
    TEST_EQUAL(getMD5Sum("123456"), "e10adc3949ba59abbe56e057f20f883e");
    TEST_EQUAL(getMD5Sum("a"), "0cc175b9c0f1b6a831c399e269772661");
    TEST_EQUAL(getMD5Sum("abc"), "900150983cd24fb0d6963f7d28e17f72");
    TEST_EQUAL(getMD5Sum("message digest"), "f96b697d7cb7938d525a2f31aaf161d0");
    TEST_EQUAL(getMD5Sum("abcdefghijklmnopqrstuvwxyz"), "c3fcd3d76192e4007dfb496cca67e13b");
    TEST_EQUAL(getMD5Sum("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"), "d174ab98d277d9f5a5611c2c9f419d9f");
    TEST_EQUAL(getMD5Sum("12345678901234567890123456789012345678901234567890123456789012345678901234567890"), "57edf4a22be3c955ac49da2e2107b67a");
  }
};

TEST_REGISTER(MD5Sum);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
