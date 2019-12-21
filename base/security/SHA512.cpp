/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/SHA512.h>
#include <base/security/Bytes.h>
#include <base/Functor.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const uint64 SHA512::K[MESSAGE_SCHEDULE] = {
  0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
  0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
  0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
  0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
  0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
  0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
  0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
  0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
  0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
  0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
  0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
  0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
  0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
  0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
  0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
  0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
  0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
  0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
  0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
  0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

SHA512::SHA512() noexcept {
  messageDigest[0] = 0x6a09e667f3bcc908; // H0
  messageDigest[1] = 0xbb67ae8584caa73b; // H1
  messageDigest[2] = 0x3c6ef372fe94f82b; // H2
  messageDigest[3] = 0xa54ff53a5f1d36f1; // H3
  messageDigest[4] = 0x510e527fade682d1; // H4
  messageDigest[5] = 0x9b05688c2b3e6c1f; // H5
  messageDigest[6] = 0x1f83d9abfb41bd6b; // H6
  messageDigest[7] = 0x5be0cd19137e2179; // H7
}

void SHA512::pushBlock(const uint8* block) noexcept {
  uint64 a = messageDigest[0]; // A = H0
  uint64 b = messageDigest[1]; // B = H1
  uint64 c = messageDigest[2]; // C = H2
  uint64 d = messageDigest[3]; // D = H3
  uint64 e = messageDigest[4]; // E = H4
  uint64 f = messageDigest[5]; // F = H5
  uint64 g = messageDigest[6]; // G = H6
  uint64 h = messageDigest[7]; // H = H7
  
  uint64 words[MESSAGE_SCHEDULE];
  for (unsigned int t = 0; t < 16; ++t) { // Copy M into W0 to W15
    words[t] = ByteIO::readUInt64(block);
    block += 8;
  }
  for (unsigned int t = 16; t < MESSAGE_SCHEDULE; ++t) {
    words[t] = sigma1(words[t-2]) + words[t-7] + sigma0(words[t-15]) + words[t-16];
  }

  for (unsigned int t = 0; t < MESSAGE_SCHEDULE; ++t) {
    uint64 T1 = h + sum1(e) + ch(e, f, g) + K[t] + words[t];
    uint64 T2 = sum0(a) + maj(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
  }
  
  messageDigest[0] += a;
  messageDigest[1] += b;
  messageDigest[2] += c;
  messageDigest[3] += d;
  messageDigest[4] += e;
  messageDigest[5] += f;
  messageDigest[6] += g;
  messageDigest[7] += h;
}

MemorySize SHA512::push(const uint8* buffer, MemorySize size)
{
  if (size > (MAXIMUM_SIZE - totalSize)) {
    throw OutOfRange();
  }
  MemorySize result = size;
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

void SHA512::pushEnd() noexcept {
  BASSERT(bytesInBuffer < BLOCK_SIZE);
  buffer[bytesInBuffer++] = 0x80; // append 0b10000000

  const unsigned int SIZE_FIELD = 128/8;
  
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
  buffer[bytesInBuffer++] = 0x00; // most significant word of field is currently not used
  buffer[bytesInBuffer++] = 0x00;
  buffer[bytesInBuffer++] = 0x00;
  buffer[bytesInBuffer++] = 0x00;
  buffer[bytesInBuffer++] = 0x00;
  buffer[bytesInBuffer++] = 0x00;
  buffer[bytesInBuffer++] = 0x00;
  buffer[bytesInBuffer++] = 0x00;
  uint64 temp = totalSize * 8;
  for (unsigned int i = sizeof(temp); i > 0;) {
    --i;
    buffer[bytesInBuffer++] = temp >> (i << 3);
  }
  pushBlock(buffer);
  bytesInBuffer = 0;
}

String SHA512::getValue() const noexcept
{
  uint8 temp[sizeof(messageDigest)];
  Bytes::convertWordsToBytesBigEndian(temp, messageDigest);
  return Bytes::getAsHex(temp, sizeof(temp));
}

String SHA512::getBase64() const noexcept
{
  uint8 temp[sizeof(messageDigest)];
  Bytes::convertWordsToBytesBigEndian(temp, messageDigest);
  return Bytes::getAsBase64(temp, sizeof(temp));
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(SHA512) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base/security");
  TEST_IMPACT(SECURITY);

  String getSHA512(const uint8* buffer, MemorySize size)
  {
    SHA512 digest;
    digest.push(buffer, size);
    digest.pushEnd();
    return digest.getValue();
  }

  String getSHA512(const char* text)
  {
    return getSHA512(reinterpret_cast<const uint8*>(text), getNullTerminatedLength(text));
  }

  void run() override
  {
    TEST_EQUAL(getSHA512(""), "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
    TEST_EQUAL(getSHA512("abc"), "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");
  }
};

TEST_REGISTER(SHA512); // Emscripten - not registering many tests
// TAG: need to force primary objects to have highest init priority

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
