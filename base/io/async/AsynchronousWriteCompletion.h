/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_WRITE_COMPLETION_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_WRITE_COMPLETION_H

#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class describes the completion status of an asynchronous write operation.

  @short Asynchronous write operation completion
  @see AsynchronousWriteEventListener
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AsynchronousWriteCompletion : public Object {
public:

  enum Flags {
    COMPLETED = 1, /**< Indicates whether or not the operation has been completed. */
    SUCCESSFUL = 2, /**< Indicates whether or not the operation was succesful. */
    ABORTED = 4 /**< Indicates when set that the operation was cancelled. */
  };
protected:
  
  /** The buffer associated with the operation. */
  const char* buffer;
  /** The desired number of bytes to be written. */
  unsigned int bytesToWrite;
  /** The offset. */
  unsigned long long offset;
  /** Specifies the number of bytes written. */
  unsigned int bytesWritten;
  /** Flags. */
  unsigned int flags;
public:

  /**
    Initialization of object.
  */
  inline AsynchronousWriteCompletion(const char* _buffer, unsigned int _bytesToWrite, unsigned long long _offset, unsigned int _bytesWritten, unsigned int _flags) throw() 
    : buffer(_buffer),
      bytesToWrite(_bytesToWrite),
      offset(_offset),
      bytesWritten(_bytesWritten),
      flags(_flags) {
  }
  
  /**
    Initialization of object from other object.
  */
  AsynchronousWriteCompletion(const AsynchronousWriteCompletion& copy) throw();

  /**
    Assignment of object by object.
  */
  AsynchronousWriteCompletion& operator=(const AsynchronousWriteCompletion& eq) throw();

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

  inline const char* getBuffer() const throw() {
    return buffer;
  }
  
  inline unsigned int getBytesToWrite() const throw() {
    return bytesToWrite;
  }
  
  inline unsigned long long getOffset() const throw() {
    return offset;
  }
  
  inline unsigned int getBytesWritten() const throw() {
    return bytesWritten;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
