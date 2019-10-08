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

#include <base/mem/ReferenceCountedObject.h>
#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Base class of all streams of bytes.
  
  @short Stream.
  @ingroup io
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Stream : public virtual ReferenceCountedObject {
public:

  /**
    Closes the stream and releases any system resources associated with the
    stream.
  */
  virtual void close() throw(IOException) = 0;

  /**
    Destroy stream object.
  */
  virtual ~Stream();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
