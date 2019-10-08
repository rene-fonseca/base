/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
  Low-level pull interface for in-memory IO.
  
  @short Pull interface
  @ingroup io
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API PullInterface {
public:
  
  /**
    Pulls up to the specified number of bytes from the stream.
    
    @param buffer The buffer to receive the pulled bytes.
    @param size The number of bytes to pull.
    @return The number of bytes pulled. 0 if the end of the stream has been
    reached.
  */
  virtual unsigned int pull(uint8* buffer, unsigned int size) /*throw(...)*/ = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
