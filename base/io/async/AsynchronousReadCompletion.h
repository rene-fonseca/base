/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_COMPLETION_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_COMPLETION_H

#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class describes the completion status of an asynchronous read operation.

  @short Asynchronous read operation completion
  @see AsynchronousReadEventListener
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AsynchronousReadCompletion : public Object {
public:

  enum Flags {
    COMPLETED = 1, /**< Indicates whether or not the operation has been completed. */
    SUCCESSFUL = 2, /**< Indicates whether or not the operation was succesful. */
    ABORTED = 4, /**< Indicates when set that the operation was cancelled. */
    END_OF_FILE = 8 /**< Indicates when set that the end of file has been reached. */
  };
protected:
  
  /** The buffer associated with the operation. */
  char* buffer;
  /** The desired number of bytes to be read. */
  unsigned int bytesToRead;
  /** The offset. */
  unsigned long long offset;
  /** Specified the number of bytes read. */
  unsigned int bytesRead;
  /** Flags. */
  unsigned int flags;
public:

  /**
    Initialization of object.
  */
  inline AsynchronousReadCompletion(char* _buffer, unsigned int _bytesToRead, unsigned long long _offset, unsigned int _bytesRead, unsigned int _flags) throw() 
    : buffer(_buffer),
      bytesToRead(_bytesToRead),
      offset(_offset),
      bytesRead(_bytesRead),
      flags(_flags) {
  }

  /**
    Initialization of object from other object.
  */
  AsynchronousReadCompletion(const AsynchronousReadCompletion& copy) throw();

  /**
    Assignment of object by object.
  */
  AsynchronousReadCompletion& operator=(const AsynchronousReadCompletion& eq) throw();

  inline bool eof() const throw() {
    return (flags & END_OF_FILE) != 0;
  }
  
  inline bool successful() const throw() {
    return (flags & SUCCESSFUL) != 0;
  }

  inline bool hasCompleted() const throw() {
    return (flags & COMPLETED) != 0;
  }
  
  inline bool isPending() const throw() {
    return (flags & COMPLETED) == 0;
  }

  inline bool wasAborted() const throw() {
    return (flags & ABORTED) != 0;
  }

  inline char* getBuffer() throw() {
    return buffer;
  }
  
  inline unsigned int getBytesToRead() const throw() {
    return bytesToRead;
  }
  
  inline unsigned long long getOffset() const throw() {
    return offset;
  }

  inline unsigned int getBytesRead() const throw() {
    return bytesRead;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
