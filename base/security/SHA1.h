/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_SECURITY__SHA1_H
#define _DK_SDU_MIP__BASE_SECURITY__SHA1_H

#include <base/string/String.h>
#include <base/OutOfRange.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Implementation of the SHA-1 message-digest algorithm (see FIPS PUB 180-1).
  This class takes a message of arbitrary length and produces a 128-bit
  "fingerprint"/"message digest" of the message. According to the RFC it is
  computationally infeasible to produce two messages having the same message
  digest, or to produce any message having a given prespecified target message
  digest.

  The SHA-1 is designed to have the following properties: it is computationally
  infeasible to find a message which corresponds to a given message digest, or
  to find two different messages which produce the same message digest.

  <pre>
  String message = "abcdefghijklmnopqrstuvwxyz";
  SHA1 checksum;
  checksum.push(pointer_cast<const byte*>(message.getElements()), message.getLength());
  checksum.pushEnd();
  fout << MESSAGE("message digest: ") << checksum.getValue() << ENDL;
  </pre>

  @short SHA-1 message-digest.
  @ingroup security
  @see MD5Sum
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class SHA1 : public Object {
public:

  /** Internal block size in bytes. */
  static const unsigned int BLOCK_SIZE = 64;
  /** Specifies the maximum length (in bytes) of the original message. */
  static const unsigned long long MAXIMUM_SIZE = (1ULL << (64 - 3)) - 1;
private:

  /** Message digest buffer. */
  unsigned int messageDigest[5];
  /** The total number of bytes pushed. */
  unsigned long long totalSize;
  /** Temporary container for incomplete 16 word block. */
  byte buffer[BLOCK_SIZE];
  /** The number of bytes in the buffer. */
  unsigned int bytesInBuffer;

  /** Push one block (16 words). */
  void pushBlock(const byte* block) throw();
public:

  /**
    Initializes object.
  */
  SHA1() throw();

  /**
    Updates the checksum corresponding to the specified data. May be called
    multiple times. Throws OutOfRange if the total number of octets exceed the
    limit specified by MAXIMUM_SIZE.

    @param buffer The buffer holding the data.
    @param size The number of octets in the buffer.
  */
  void push(const byte* buffer, unsigned int size) throw(OutOfRange);

  /**
    This function should be invoked when the entire message has been pushed.
    Do NOT use push() after invoking this function.
  */
  void pushEnd() throw();

  /**
    Returns the total size of the original message.
  */
  unsigned long long getTotalSize() const throw();

  /**
    Returns the message digest encoded in hex. This is only valid after
    pushEnd() has been invoked.
  */
  String getValue() const throw();

  /**
    Returns the message digest encoded in Base64. This is only valid after
    pushEnd() has been invoked.
  */
  String getBase64() const throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
