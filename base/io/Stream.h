/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__STREAM_H
#define _DK_SDU_MIP__BASE_IO__STREAM_H

#include <base/Object.h>
#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Base class of all streams of bytes.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Stream {
public:

  /**
    Closes the stream and releases any system resources associated with the stream.
  */
  virtual void close() throw(IOException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
