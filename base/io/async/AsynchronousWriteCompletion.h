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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class describes the completion status of an asynchronous write
  operation.

  @short Asynchronous write operation completion
  @see AsynchronousWriteEventListener
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AsynchronousWriteCompletion : public Object {
public:

  enum Flags {
    COMPLETED = 1, /**< Indicates whether or not the operation has been completed. */
    SUCCESSFUL = 2, /**< Indicates whether or not the operation was succesful. */
    ABORTED = 4 /**< Indicates when set that the operation was cancelled. */
  };
protected:
  
  /** The buffer associated with the operation. */
  const uint8* buffer = nullptr;
  /** The desired number of bytes to be written. */
  unsigned int bytesToWrite = 0;
  /** The offset. */
  unsigned long long offset = 0;
  /** Specifies the number of bytes written. */
  unsigned int bytesWritten = 0;
  /** Flags. */
  unsigned int flags = 0;
public:

  /**
    Initialization of object.
  */
  inline AsynchronousWriteCompletion(
    const uint8* _buffer,
    unsigned int _bytesToWrite,
    unsigned long long _offset,
    unsigned int _bytesWritten,
    unsigned int _flags) noexcept 
    : buffer(_buffer),
      bytesToWrite(_bytesToWrite),
      offset(_offset),
      bytesWritten(_bytesWritten),
      flags(_flags) {
  }
  
  /**
    Initialization of object from other object.
  */
  AsynchronousWriteCompletion(const AsynchronousWriteCompletion& copy) noexcept;

  /**
    Assignment of object by object.
  */
  AsynchronousWriteCompletion& operator=(const AsynchronousWriteCompletion& assign) noexcept;

  inline bool successful() const noexcept
  {
    return (flags & SUCCESSFUL) != 0;
  }

  inline bool hasCompleted() const noexcept
  {
    return (flags & COMPLETED) != 0;
  }
  
  inline bool isPending() const noexcept
  {
    return (flags & COMPLETED) == 0;
  }

  inline bool wasAborted() const noexcept
  {
    return (flags & ABORTED) != 0;
  }

  inline const uint8* getBuffer() const noexcept
  {
    return buffer;
  }
  
  inline unsigned int getBytesToWrite() const noexcept
  {
    return bytesToWrite;
  }
  
  inline unsigned long long getOffset() const noexcept
  {
    return offset;
  }
  
  inline unsigned int getBytesWritten() const noexcept
  {
    return bytesWritten;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
