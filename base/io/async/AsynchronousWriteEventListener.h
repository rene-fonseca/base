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

#include <base/io/async/AsynchronousWriteCompletion.h>
#include <base/DynamicObject.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short Event listener for asynchronous write operations.
  @see AsynchronousWriteCompletion
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AsynchronousWriteEventListener : public virtual DynamicObject {
public:

  virtual void asynchronousCompletion(
    const AsynchronousWriteCompletion& completion) noexcept = 0;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
