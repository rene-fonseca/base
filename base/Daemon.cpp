/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Daemon.h>
#include <base/ResourceException.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(__win32__)

bool Daemon::daemonize() throw(ResourceException) {
  return true;
}

#else // __unix__

bool Daemon::daemonize() throw(ResourceException) {
  switch(fork()) {
  case 0:
    setsid();
    return true;
  case -1:
    throw ResourceException("Unable to daemonize process");
  default:
    return false;
  }
}

#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
