/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_EVENT_LISTENER_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_EVENT_LISTENER_H

#include <base/io/async/AsynchronousReadCompletion.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short Event listener for asynchronous read operations.
  @see AsynchronousReadCompletion
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AsynchronousReadEventListener {
public:
  
  virtual void asynchronousCompletion(const AsynchronousReadCompletion& completion) throw() = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
