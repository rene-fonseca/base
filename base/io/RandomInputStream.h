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
  Random generator input stream.

  @ingroup io
*/

class _COM_AZURE_DEV__BASE__API RandomInputStream : public InputStream {
private:

  void* engine = nullptr;
  RandomInputStream(const RandomInputStream&) = delete;
  RandomInputStream& operator=(const RandomInputStream&) = delete;
public:

  enum {
    USE_RANDOM_SEED = 0
  };

  /** Initializes random input stream. If seed is set to USE_RANDOM_SEED (0) a random seed will be used. */
  RandomInputStream(uint32 seed = USE_RANDOM_SEED);
  
  ~RandomInputStream();
  
  unsigned int available() const noexcept override;

  unsigned int read(uint8* buffer, const unsigned int _size, bool nonblocking) noexcept override;

  unsigned int skip(unsigned int count) noexcept override;

  void close() noexcept override;

  void wait() const noexcept override;

  bool wait(unsigned int timeout) const noexcept override;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
