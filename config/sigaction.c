/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program links if sigaction is provided by the c library.
*/

#include <signal.h>

void handler(int signal, siginfo_t* info, void* context) {
}

int main() {
  struct sigaction action;
  action.sa_flags = SA_SIGINFO;
  sigemptyset(&action.sa_mask);
  action.sa_sigaction = handler;
  sigaction(SIGSEGV, &action, 0);
  return 0;
}
