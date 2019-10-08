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

#include <base/io/async/AsynchronousOperation.h>
#include <base/io/async/AsynchronousReadContext.h>
#include <base/mem/Reference.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class AsynchronousReadContext;

/**
  Asynchronous reading operation returned by an asynchronous IO stream.
  
  @short Asynchronous read operation.
  @version 1.0
*/
class AsynchronousReadOperation : public AsynchronousOperation {
private:

  /** Context of operation. */
  Reference<AsynchronousReadContext> context;
public:

  /**
    Initializes the handle as invalid.
  */
  inline AsynchronousReadOperation() throw() {
  }
  
  /**
    Initializes the handle to the read operation.
  */
  inline AsynchronousReadOperation(AsynchronousReadContext* _context) throw()
    : context(_context) {
  }
  
  /**
    Initialization of object from other object.
  */
  inline AsynchronousReadOperation(
    const AsynchronousReadOperation& copy) throw() {
    context = copy.context;
  }

  /**
    Assignment of object by object.
  */
  inline AsynchronousReadOperation& operator=(
    const AsynchronousReadOperation& eq) throw() {
    context = eq.context;
    return *this;
  }

  /**
    Explicit release of the handle to the asynchronous operation. This does not
    abort the asynchronous operation.
  */
  inline void release() throw() {
    context = nullptr;
  }
  
  /**
    Returns the completion information associated with the asynchronous
    operation. This method is normally only used for debugging.
  */
  AsynchronousReadCompletion getCompletion() const throw(NullPointer);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
