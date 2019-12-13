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

#include <base/Object.h>
#include <base/ResourceException.h>
#include <base/SingletonException.h>
#include <base/concurrency/Thread.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Class used for daemonizing a process.

  @short Daemon.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Daemon : public Object {
public:

  /**
    Daemonizes the current process.

    @param runnable The entry function of the daemonized process.
  */
  Daemon(Runnable* runnable);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static void install();
#endif
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
