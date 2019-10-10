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

#include <base/io/async/AsynchronousReadEventListener.h>
#include <base/io/async/AsynchronousReadCompletion.h>
#include <base/io/async/AsynchronousException.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API AsynchronousReadContext : public Object, public ReferenceCountedObject {
protected:  

  /** Self-reference used to protect the object from premature destruction. */
  Reference<AsynchronousReadContext> selfReference;
  
  inline AsynchronousReadContext() throw() : selfReference(this) {
  }
public:
  
  virtual AsynchronousReadCompletion getCompletion() const throw() = 0;

  virtual ~AsynchronousReadContext();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
