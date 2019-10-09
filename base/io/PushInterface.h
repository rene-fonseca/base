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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Low-level push interface for in-memory IO.
  
  @short Push interface
  @ingroup io
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API PushInterface {
public:

  /**
    Forces any buffered bytes to be written out. This method is ignored by
    unbuffered streams.
  */
  virtual void flush() /*throw(...)*/;
  
  /**
    Pushes the specified number of bytes onto the stream.
    
    @param buffer The buffer containing the bytes to be pushed.
    @param size The number of bytes to be pushed.
    @return The number of bytes pushed.
  */
  virtual unsigned int push(const uint8* buffer, unsigned int size) = 0;
  
  /**
    This method should be invoked when the end has been reached. This method may
    be ignored by some implementations.
  */
  virtual void pushEnd() /*throw(...)*/;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
