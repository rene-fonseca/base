/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__STRING_OUTPUT_STREAM_H
#define _DK_SDU_MIP__BASE_STRING__STRING_OUTPUT_STREAM_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/io/OutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class StringOutputStreamWrapper : public virtual Object, public virtual OutputStream {
private:

  /** The string holding the stream data. */
  String string;
  /** Specifies whether the stream has been closed. */
  bool closed;
public:

  inline explicit StringOutputStreamWrapper() throw() : closed(false) {}

  void close() throw(IOException);

  void flush() throw(IOException);

  unsigned int write(const char* buffer, unsigned int size, bool nonblocking = false) throw(IOException);

  inline unsigned int getGranularity() const throw() {return string.getGranularity();}

  inline void setGranularity(unsigned int granularity) throw() {string.setGranularity(granularity);}

  inline String getString() const throw() {return string;}
};



// Helper class used by StringOutputStream.
class StringOutputStreamImpl {
protected:

  StringOutputStreamWrapper stream;
public:

  inline StringOutputStreamImpl() {}
};



/**
  A string output stream is a format output stream that directs the stream data
  into a string.

  @short String output stream.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class StringOutputStream : protected StringOutputStreamImpl, public FormatOutputStream {
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
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
