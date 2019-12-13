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

#include <base/Object.h>
#include <base/string/WideString.h>
#include <base/io/OutputStream.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API WideStringOutputStreamWrapper : public virtual Object,
                                                                public virtual OutputStream {
private:

  /** The string holding the stream data. */
  WideString string;
  /** Specifies whether the stream has been closed. */
  bool closed = false;
public:

  inline explicit WideStringOutputStreamWrapper() noexcept
  {
  }

  /**
    Reserves capacity in the internal buffer.
  */
  void ensureCapacity(MemorySize capacity);

  void close();

  void flush();

  void restart();

  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

#if 0
  inline unsigned int getGranularity() const noexcept
  {
    return string.getGranularity();
  }

  inline void setGranularity(unsigned int granularity) noexcept
  {
    string.setGranularity(granularity);
  }
#endif

  inline const WideString& getString() const noexcept
  {
    return string;
  }
  
  inline ~WideStringOutputStreamWrapper()
  {
  }
};



// Helper class used by WideStringOutputStream.
class _COM_AZURE_DEV__BASE__API WideStringOutputStreamImpl {
protected:

  WideStringOutputStreamWrapper stream;
public:

  inline WideStringOutputStreamImpl() noexcept
  {
  }
};



/**
  A string output stream is a format output stream that directs the stream data
  into a string.

  @short String output stream.
  @version 1.0
*/

// TAG: need WideFormatOutputStream
class _COM_AZURE_DEV__BASE__API WideStringOutputStream
  : protected WideStringOutputStreamImpl, public FormatOutputStream {
public:

  /** Specifies the default amount of memory by which the capacity is increased. */
  static const unsigned int DEFAULT_GRANULARITY = 1024;
public:

  /**
    Initializes string output stream using the default incrementation value (i.e. DEFAULT_GRANULARITY).
  */
  WideStringOutputStream();

  /**
    Initializes string output stream.

    @param granularity Specifies the default amount of memory by which the capacity of the string is increased.
  */
  explicit WideStringOutputStream(unsigned int granularity);

  /**
    Reserves capacity in the internal buffer.
  */
  void ensureCapacity(MemorySize capacity);

  /**
    Returns the granularity.
  */
  unsigned int getGranularity() const noexcept;

  /**
    Sets the granularity.
  */
  void setGranularity(unsigned int granularity) noexcept;

  /**
    Returns the string associated with the stream.
  */
  const String& getString() const noexcept;

  /**
    Returns the string associated with the stream and restarts the stream.
  */
  String toString();

  /**
    Destroy stream object.
  */
  ~WideStringOutputStream()
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
