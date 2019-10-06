/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Class used for daemonizing a process.

  @short Daemon.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Daemon : public Object {
public:

  /**
    Daemonizes the current process.

    @param runnable The entry function of the daemonized process.
  */
  Daemon(Runnable* runnable) throw(SingletonException, ResourceException);

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static void install();
#endif
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
