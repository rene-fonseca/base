/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/io/async/AsynchronousWriteEventListener.h>
#include <base/io/async/AsynchronousException.h>
#include <base/mem/Reference.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Asynchronous write context.

*/
class AsynchronousWriteContext : public Object, public ReferenceCountedObject {
protected:
  
  /** Self-reference used to protect the object from premature destruction. */
  Reference<AsynchronousWriteContext> selfReference;

  inline AsynchronousWriteContext() throw() : selfReference(this) {
  }
public:

  virtual AsynchronousWriteCompletion getCompletion() const throw() = 0;
  
  virtual ~AsynchronousWriteContext();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
