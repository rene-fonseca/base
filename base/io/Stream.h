/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__STREAM_H
#define _DK_SDU_MIP__BASE_IO__STREAM_H

#include <base/Object.h>
#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Base class of all streams of bytes.

  @author Ren� M�ller Fonseca
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
