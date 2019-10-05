/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_WRITE_EVENT_LISTENER_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_WRITE_EVENT_LISTENER_H

#include <base/io/async/AsynchronousWriteCompletion.h>
#include <base/DynamicObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Event listener for asynchronous write operations.
  @see AsynchronousWriteCompletion
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AsynchronousWriteEventListener : public virtual DynamicObject {
public:

  virtual void asynchronousCompletion(
    const AsynchronousWriteCompletion& completion) throw() = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
