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

#include <base/io/IOException.h>
#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Low-level push interface for in-memory IO.
  
  @short Push interface
  @ingroup io
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API PushInterface {
public:

  /**
    Forces any buffered bytes to be written out. This method is ignored by
    unbuffered streams.
  */
  virtual void flush();

  /**
    Begins the pushes.
    
    @param totalSize The total number of bytes to be pushed.
  */
  virtual bool pushBegin(uint64 totalSize);

  /**
    Pushes the specified number of bytes onto the stream.
    
    @param buffer The buffer containing the bytes to be pushed.
    @param size The number of bytes to be pushed.
    @return The number of bytes pushed.
  */
  virtual MemorySize push(const uint8* buffer, MemorySize size) = 0;
  
  /**
    This method should be invoked when the end has been reached. This method may
    be ignored by some implementations.
  */
  virtual void pushEnd();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
