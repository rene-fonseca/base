/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_WRITE_OPERATION_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_WRITE_OPERATION_H

#include <base/io/async/AsynchronousOperation.h>
#include <base/io/async/AsynchronousWriteContext.h>
#include <base/mem/ReferenceCountedObjectPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class AsynchronousWriteContext;

/**
  Asynchronous writing operation returned by an asynchronous IO stream.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class AsynchronousWriteOperation : public AsynchronousOperation {
private:

  /** Context of operation. */
  ReferenceCountedObjectPointer<AsynchronousWriteContext> context;
public:

  inline AsynchronousWriteOperation(AsynchronousWriteContext* _context) throw()
    : context(_context) {
  }

  /**
    Initializes invalid object.
  */
  inline AsynchronousWriteOperation() throw() : context(0) {
  }

  /*
    Initialization by copying from other object.
  */
  inline AsynchronousWriteOperation(const AsynchronousWriteOperation& copy) throw() {
    this->context = copy.context;
  }
  
  /*
    Assignment operator.
  */
  inline AsynchronousWriteOperation& operator=(const AsynchronousWriteOperation& eq) throw() {
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
  AsynchronousWriteCompletion getCompletion() const throw(NullPointer);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
