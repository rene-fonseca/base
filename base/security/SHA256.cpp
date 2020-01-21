/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/SHA256.h>
#include <base/security/Bytes.h>
#include <base/Functor.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const uint32 SHA256::K[SHA256::MESSAGE_SCHEDULE] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

SHA256::SHA256() noexcept
{
  clear(buffer);
  messageDigest[0] = 0x6a09e667; // H0
  messageDigest[1] = 0xbb67ae85; // H1
  messageDigest[2] = 0x3c6ef372; // H2
  messageDigest[3] = 0xa54ff53a; // H3
  messageDigest[4] = 0x510e527f; // H4
  messageDigest[5] = 0x9b05688c; // H5
  messageDigest[6] = 0x1f83d9ab; // H6
  messageDigest[7] = 0x5be0cd19; // H7
}

void SHA256::pushBlock(const uint8* block) noexcept
{
  uint32 a = messageDigest[0]; // A = H0
  uint32 b = messageDigest[1]; // B = H1
  uint32 c = messageDigest[2]; // C = H2
  uint32 d = messageDigest[3]; // D = H3
  uint32 e = messageDigest[4]; // E = H4
  uint32 f = messageDigest[5]; // F = H5
  uint32 g = messageDigest[6]; // G = H6
  uint32 h = messageDigest[7]; // H = H7
  
  uint32 words[MESSAGE_SCHEDULE];
  for (unsigned int t = 0; t < 16; ++t) { // Copy M into W0 to W15
    words[t] = ByteIO::readUInt32(block);
    block += 4;
  }
  for (unsigned int t = 16; t < MESSAGE_SCHEDULE; ++t) {
    words[t] = sigma1(words[t-2]) + words[t-7] + sigma0(words[t-15]) + words[t-16];
  }

  for (unsigned int t = 0; t < MESSAGE_SCHEDULE; ++t) {
    uint32 T1 = h + sum1(e) + ch(e, f, g) + K[t] + words[t];
    uint32 T2 = sum0(a) + maj(a, b, c);
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

MemorySize SHA256::push(const uint8* buffer, MemorySize size)
{
  if (size > (MAXIMUM_SIZE - totalSize)) {
    _throw OutOfRange();
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

void SHA256::pushEnd() noexcept
{
  BASSERT(bytesInBuffer < BLOCK_SIZE);
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

String SHA256::getValue() const noexcept
{
  uint8 temp[sizeof(messageDigest)];
  Bytes::convertWordsToBytesBigEndian(temp, messageDigest);
  return Bytes::getAsHex(temp, sizeof(temp));
}

String SHA256::getBase64() const noexcept
{
  uint8 temp[sizeof(messageDigest)];
  Bytes::convertWordsToBytesBigEndian(temp, messageDigest);
  return Bytes::getAsBase64(temp, sizeof(temp));
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(SHA256) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base/security");
  TEST_IMPACT(SECURITY)

  String getSHA256(const uint8* buffer, MemorySize size)
  {
    SHA256 digest;
    digest.push(buffer, size);
    digest.pushEnd();
    return digest.getValue();
  }

  String getSHA256(const char* text)
  {
    return getSHA256(reinterpret_cast<const uint8*>(text), getNullTerminatedLength(text));
  }

  void run() override
  {
    TEST_EQUAL(getSHA256(""), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    TEST_EQUAL(getSHA256("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
  }
};

TEST_REGISTER(SHA256);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
