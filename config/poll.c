/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program links if poll is provided by the c library.
*/

#define _XOPEN_SOURCE 600

#include <sys/poll.h>

int main() {
  int result;
  int milliseconds;
  struct pollfd fds[1];
  
  fds[0].fd = 0;
  
  fds[0].events = POLLRDNORM;
  fds[0].events = POLLRDBAND;
  fds[0].events = POLLPRI;
  fds[0].events = POLLWRNORM;
  fds[0].events = POLLWRBAND;
  
  fds[0].revents = POLLRDNORM;
  fds[0].revents = POLLRDBAND;
  fds[0].revents = POLLPRI;
  fds[0].revents = POLLWRNORM;
  fds[0].revents = POLLWRBAND;
  fds[0].revents = POLLERR;
  fds[0].revents = POLLHUP;
  
  milliseconds = 0;
  int result = poll(fds, 1, milliseconds);
  
  return 0;
}
