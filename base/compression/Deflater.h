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

#include <base/io/PushInterface.h>
#include <base/io/PullInterface.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Interface implemented by compression classes. The uncompressed and compressed
  data is pushed onto and pulled from the stream, respectively. push() may be
  invoked in the initial mode of the deflation stream unless noted otherwise.
  
  @short Deflater push stream interface.
  @ingroup compression
  @version 1.0
*/
class _COM_AZURE_DEV__BASE__API Deflater : public virtual PushInterface, public virtual PullInterface {
public:
  
  /**
    Returns true if the end has been reached (no more uncompressed data may be
    pulled from the stream). This always returns false until pushEnd() has been
    invoked.
  */
  virtual bool atEnd() const = 0;
  
  /**
    Flushes any uncompressed data onto the stream. The stream has been flushed
    when pull() returns 0. One or more invocations of pull() is required. push()
    may only be invoked hereafter.
  */
  virtual void flush() = 0;

  virtual ~Deflater()
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
