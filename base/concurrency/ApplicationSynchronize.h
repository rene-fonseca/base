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

#include <base/concurrency/Synchronizeable.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is used to synchronize executing contexts exclusively using the
  application global lock. Do NOT overuse to avoid crosstalk between threads.

  @ingroup concurrency
*/

class _COM_AZURE_DEV__BASE__API ApplicationSynchronize {
private:

  ApplicationSynchronize(const ApplicationSynchronize& copy);
  ApplicationSynchronize& operator=(const ApplicationSynchronize& assign);
public:

  ApplicationSynchronize();

  ~ApplicationSynchronize();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
