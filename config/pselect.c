/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program links if pselect is provided by the c library.
*/

#define _XOPEN_SOURCE 600

#include <sys/select.h>

#if !defined(FD_SETSIZE)
  #warning FD_SETSIZE is not defined
#endif

int main() {
  int result;
  struct timespec timeout;
  fd_set readfds;
  fd_set writefds;
  fd_set errorfds;

  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&errorfds);
  
  FD_SET(0, &readfds);
  if (!FD_ISSET(0, &readfds)) {
    return 1;
  }
  FD_CLR(0, &readfds);
  if (FD_ISSET(0, &readfds)) {
    return 1;
  }
  
  timeout.tv_sec = 0;
  timeout.tv_nsec = 1;
  result = pselect(1, &readfds, &writefds, &errorfds, &timeout, 0);
  if (result < -1) {
    return 1;
  }
  return 0;
}
