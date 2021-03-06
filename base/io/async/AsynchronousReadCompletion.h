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
  This class describes the completion status of an asynchronous read operation.

  @short Asynchronous read operation completion
  @see AsynchronousReadEventListener
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AsynchronousReadCompletion : public Object {
public:

  enum Flags {
    COMPLETED = 1, /**< Indicates whether or not the operation has been completed. */
    SUCCESSFUL = 2, /**< Indicates whether or not the operation was succesful. */
    ABORTED = 4, /**< Indicates when set that the operation was cancelled. */
    END_OF_FILE = 8 /**< Indicates when set that the end of file has been reached. */
  };
protected:
  
  /** The buffer associated with the operation. */
  uint8* buffer = nullptr;
  /** The desired number of bytes to be read. */
  unsigned int bytesToRead = 0;
  /** The offset. */
  unsigned long long offset = 0;
  /** Specified the number of bytes read. */
  unsigned int bytesRead = 0;
  /** Flags. */
  unsigned int flags = 0;
public:

  /**
    Initialization of object.
  */
  inline AsynchronousReadCompletion(
    uint8* _buffer,
    unsigned int _bytesToRead,
    unsigned long long _offset,
    unsigned int _bytesRead,
    unsigned int _flags) noexcept 
    : buffer(_buffer),
      bytesToRead(_bytesToRead),
      offset(_offset),
      bytesRead(_bytesRead),
      flags(_flags) {
  }

  /**
    Initialization of object from other object.
  */
  AsynchronousReadCompletion(const AsynchronousReadCompletion& copy) noexcept;

  /**
    Assignment of object by object.
  */
  AsynchronousReadCompletion& operator=(const AsynchronousReadCompletion& assign) noexcept;

  inline bool eof() const noexcept
  {
    return (flags & END_OF_FILE) != 0;
  }
  
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

  inline uint8* getBuffer() noexcept
  {
    return buffer;
  }
  
  inline unsigned int getBytesToRead() const noexcept
  {
    return bytesToRead;
  }
  
  inline unsigned long long getOffset() const noexcept
  {
    return offset;
  }

  inline unsigned int getBytesRead() const noexcept
  {
    return bytesRead;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
