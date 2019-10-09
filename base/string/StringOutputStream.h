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
#include <base/string/String.h>
#include <base/io/OutputStream.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class _DK_SDU_MIP__BASE__API StringOutputStreamWrapper : public virtual Object,
                                                         public virtual OutputStream {
private:

  /** The string holding the stream data. */
  String string;
  /** Specifies whether the stream has been closed. */
  bool closed = false;
public:

  inline explicit StringOutputStreamWrapper() throw() : closed(false) {
  }

  void close() throw(IOException);

  void flush() throw(IOException);

  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false) throw(IOException);

  inline unsigned int getGranularity() const throw() {
    return string.getGranularity();
  }

  inline void setGranularity(unsigned int granularity) throw() {
    string.setGranularity(granularity);
  }

  inline String getString() const throw() {
    return string;
  }
  
  inline ~StringOutputStreamWrapper() {
  }
};



// Helper class used by StringOutputStream.
class _DK_SDU_MIP__BASE__API StringOutputStreamImpl {
protected:

  StringOutputStreamWrapper stream;
public:

  inline StringOutputStreamImpl() throw() {
  }
};



/**
  A string output stream is a format output stream that directs the stream data
  into a string.

  @short String output stream.
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API StringOutputStream : protected StringOutputStreamImpl, public FormatOutputStream {
public:

  /** Specifies the default amount of memory by which the capacity is increased. */
  static const unsigned int DEFAULT_GRANULARITY = 1024;
public:

  /**
    Initializes string output stream using the default incrementation value (i.e. DEFAULT_GRANULARITY).
  */
  StringOutputStream() throw(BindException);

  /**
    Initializes string output stream.

    @param granularity Specifies the default amount of memory by which the capacity of the string is increased.
  */
  explicit StringOutputStream(unsigned int granularity) throw(BindException);

  /**
    Returns the granularity.
  */
  unsigned int getGranularity() const throw();

  /**
    Sets the granularity.
  */
  void setGranularity(unsigned int granularity) throw();

  /**
    Returns the string associated with the stream.
  */
  String getString() const throw();

  /**
    Destroy stream object.
  */
  ~StringOutputStream() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
