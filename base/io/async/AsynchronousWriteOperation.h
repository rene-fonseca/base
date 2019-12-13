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

#include <base/io/async/AsynchronousOperation.h>
#include <base/io/async/AsynchronousWriteContext.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class AsynchronousWriteContext;

/**
  Asynchronous writing operation returned by an asynchronous IO stream.

  @short Asynchronous write operation.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AsynchronousWriteOperation : public AsynchronousOperation {
private:

  /** Context of operation. */
  Reference<AsynchronousWriteContext> context;
public:

  /**
    Initializes invalid object.
  */
  inline AsynchronousWriteOperation() noexcept
  {
  }

  inline AsynchronousWriteOperation(AsynchronousWriteContext* _context) noexcept
    : context(_context)
  {
  }

  /*
    Initialization by copying from other object.
  */
  inline AsynchronousWriteOperation(const AsynchronousWriteOperation& copy) noexcept
  {
    this->context = copy.context;
  }
  
  /*
    Assignment operator.
  */
  inline AsynchronousWriteOperation& operator=(const AsynchronousWriteOperation& assign) noexcept
  {
    this->context = assign.context;
    return *this;
  }

  /**
    Explicit release of the handle to the asynchronous operation. This does not
    abort the asynchronous operation.
  */
  inline void release() noexcept
  {
    context = 0;
  }

  /**
    Returns the completion information associated with the asynchronous
    operation. This method is normally only used for debugging.
  */
  AsynchronousWriteCompletion getCompletion() const;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
