/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__STREAM_H
#define _DK_SDU_MIP__BASE_IO__STREAM_H

#include "base/Object.h"
#include "IOException.h"

/**
  Base class of all streams of bytes.

  @author René Møller Fonseca
  @version 1.0
*/

class Stream {
public:

  /**
    Closes the stream and releases any system resources associated with the stream.
  */
  virtual void close() throw(IOException) = 0;
};

#endif
