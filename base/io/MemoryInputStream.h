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

#include <base/io/InputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Memory input stream.

  @ingroup io
*/

class _COM_AZURE_DEV__BASE__API MemoryInputStream : public InputStream {
private:

  const uint8* src = nullptr;
  const uint8* end = nullptr;
public:

  inline MemoryInputStream()
  {
  }

  inline MemoryInputStream(const uint8* _src, const uint8* _end)
    : src(_src), end(_end)
  {
  }

  inline MemoryInputStream(const String& text)
  {
    src = text.getBytes();
    end = src + text.getLength();
  }

  inline unsigned int available() const noexcept
  {
    return end - src;
  }
  
  unsigned int read(uint8* dest, unsigned int size, bool nonblocking) noexcept;

  inline unsigned int skip(unsigned int count) noexcept
  {
    count = minimum<unsigned int>(count, end - src);
    src += count;
    return count;
  }

  inline void close() noexcept
  {
    src = nullptr; 
    end = nullptr;
  }

  inline void wait() const noexcept
  {
  }

  inline bool wait(unsigned int timeout) const noexcept
  {
    return true;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
