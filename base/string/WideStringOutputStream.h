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

/** Internal string output stream. */
class _COM_AZURE_DEV__BASE__API WideStringOutputStreamWrapper : public virtual Object,
                                                                public virtual OutputStream {
private:

  /** The string holding the stream data. */
  String string;
  /** Specifies whether the stream has been closed. */
  bool closed = false;
public:

  /** Initializes buffer. */
  explicit WideStringOutputStreamWrapper() noexcept;

  /**
    Reserves capacity in the internal buffer.
  */
  void ensureCapacity(MemorySize capacity);

  /** Close stream. */
  void close();

  /** Flush stream. */
  void flush();

  /** Restarts stream. */
  void restart();

  /** Writes to string. */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /** Returns string. */
  inline const String& getString() const noexcept
  {
    return string;
  }
  
  /** Destructs buffer. */
  ~WideStringOutputStreamWrapper();
};



/** Helper class used by WideStringOutputStream. */
class _COM_AZURE_DEV__BASE__API WideStringOutputStreamImpl {
protected:

  WideStringOutputStreamWrapper stream;
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

  /** Specifies the default capacity. */
  static const MemorySize DEFAULT_CAPACITY = 1024;
public:

  /**
    Initializes string output stream.
  */
  WideStringOutputStream();

  /**
    Initializes string output stream.

    @param capacity Specifies the initial capacity.
  */
  explicit WideStringOutputStream(MemorySize capacity);

  /**
    Reserves capacity in the internal buffer.
  */
  void ensureCapacity(MemorySize capacity);

  /**
    Returns the string associated with the stream.
  */
  const String& getString() const noexcept;

  /**
    Returns the string associated with the stream and restarts the stream.
  */
  String toString() noexcept;

  /**
    Destroy stream object.
  */
  ~WideStringOutputStream()
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
