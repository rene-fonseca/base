/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__DAEMON_H
#define _DK_SDU_MIP__BASE__DAEMON_H

#include <base/Object.h>
#include <base/ResourceException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Class used for daemonizing a process.

  @short Daemon.
  @author René Møller Fonseca
*/

class Daemon : public Object {
public:

  static bool daemonize() throw(ResourceException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
