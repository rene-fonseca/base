/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__MD5SUM_H
#define _DK_SDU_MIP__BASE_SECURITY__MD5SUM_H

#include <base/Type.h>
#include <base/string/String.h>
#include <base/OutOfRange.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of the MD5 Message-Digest Algorithm (see RFC 1321).
  128 bit checksum.

  @short MD5 Message-Digest checksum calculator.
  @author René Møller Fonseca
  @version 1.0
*/

class MD5Sum : public Object {
public:

  /** Internal block size in bytes. */
  static const unsigned int BLOCK_SIZE = 64;
  /** Specifies the maximum size of the original message. */
  static const unsigned long long MAXIMUM_SIZE = UnsignedLongLong::MAXIMUM/8;
private:

  /** Message digest buffer. */
  unsigned int messageDigest[4];
  /** The total number of bytes pushed. */
  unsigned long long totalSize;
  /** Temporary container for incomplete 16 word blocks. */
  byte buffer[BLOCK_SIZE];
  /** The number of bytes in the buffer. */
  unsigned int bytesInBuffer;

  /** Push one block (16 words). */
  void pushBlock(const byte* block) throw();
public:

  /**
    Initialize object.
  */
  MD5Sum() throw();

  /**
    Updates the checksum corresponding to the specified data. May be called
    multiple times. Throws OutOfRange if the total number of octets exceed the
    limit specified by MAXIMUM_SIZE.

    @param buffer The buffer holding the data.
    @param size The number of octets in the buffer.
  */
  void push(const byte* buffer, unsigned int size) throw(OutOfRange);

  /**
    Tell object that the end has been reached. Do NOT use push() after invoking
    this function.
  */
  void pushEnd() throw();

  /**
    Returns the total size of the original message.
  */
  unsigned long long getTotalSize() const throw();

  /**
    Returns the hash value in hex. This is only valid after pushEnd() has been invoked.
  */
  String getValue() const throw();

  /**
    Returns the hash value . This is only valid after pushEnd() has been invoked.
  */
  String getBase64() const throw();

  /**
    Resets the checksum. This function is used after a pushEnd() to start a new
    checksum calculation.
  */
  void reset() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
