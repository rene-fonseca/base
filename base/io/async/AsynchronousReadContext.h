/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_CONTEXT_H
#define _DK_SDU_MIP__BASE_IO_ASYNC__ASYNCHRONOUS_READ_CONTEXT_H

#include <base/io/async/AsynchronousReadEventListener.h>
#include <base/io/async/AsynchronousReadCompletion.h>
#include <base/io/async/AsynchronousException.h>
#include <base/mem/Reference.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class AsynchronousReadContext : public Object, public ReferenceCountedObject {
protected:  

  /** Self-reference used to protect the object from premature destruction. */
  Reference<AsynchronousReadContext> selfReference;
  
  inline AsynchronousReadContext() throw() : selfReference(this) {
  }
public:
  
  virtual AsynchronousReadCompletion getCompletion() const throw() = 0;

  virtual ~AsynchronousReadContext() throw(AsynchronousException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
