/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/security/Adler32.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Adler32::Adler32() noexcept {
}

unsigned int Adler32::push(const uint8* buffer, unsigned int size) noexcept {
  const uint8* const end = buffer + size;
  uint32 s1 = checksum & 0xffff;
  uint32 s2 = (checksum >> 16) & 0xffff;
  while (buffer < end) {
    s1 = (s1 + *buffer++) % BASE;
    s2 = (s2 + s1) % BASE;
  }
  checksum = (s2 << 16) + s1;
  totalSize += size;
  return size;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Adler32) : public UnitTest {
public:

  TEST_PRIORITY(50);
  TEST_PROJECT("base/security");
  TEST_IMPACT(SECURITY);

  uint32 getAdler32(const uint8* buffer, MemorySize size)
  {
    Adler32 digest;
    digest.push(buffer, size);
    digest.pushEnd();
    return digest.getValue();
  }

  uint32 getAdler32(const char* text)
  {
    return getAdler32(reinterpret_cast<const uint8*>(text), getNullTerminatedLength(text));
  }

  void run() override
  {
    TEST_EQUAL(getAdler32(""), 0x00000001);
    TEST_EQUAL(getAdler32("abc"), 0x024d0127);
    TEST_EQUAL(getAdler32("The quick brown fox jumps over the lazy dog"), 0x5bdc0fda);
  }
};

TEST_REGISTER(Adler32);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
