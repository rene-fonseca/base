/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_OPERATION_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_OPERATION_H

#include <base/io/async/AsynchronousOperation.h>
#include <base/io/async/AsynchronousReadContext.h>
#include <base/mem/ReferenceCountedObjectPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class AsynchronousReadContext;

/**
  Asynchronous reading operation returned by an asynchronous IO stream.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
class AsynchronousReadOperation : public AsynchronousOperation {
private:

  /** Context of operation. */
  ReferenceCountedObjectPointer<AsynchronousReadContext> context;
public:

  inline AsynchronousReadOperation(AsynchronousReadContext* _context) throw() : context(_context) {}

  /**
    Initializes invalid object.
  */
  inline AsynchronousReadOperation() throw() : context(0) {}

  /*
    Initialization by copying from other object.
  */
  inline AsynchronousReadOperation(const AsynchronousReadOperation& copy) throw() {
    this->context = copy.context;
  }

  /*
    Assignment operator.
  */
  inline AsynchronousReadOperation& operator=(const AsynchronousReadOperation& eq) throw() {
    this->context = eq.context;
    return *this;
  }

  /**
    Explicit release of the handle to the asynchronous operation. This does not
    abort the asynchronous operation.
  */
  inline void release() throw() {
    context = 0;
  }
  
  /**
    Returns the completion information associated with the asynchronous
    operation. This method is normally only used for debugging.
  */
  AsynchronousReadCompletion getCompletion() const throw(NullPointer);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
