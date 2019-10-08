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
  Compiles and runs if posix thread is supported. Returns 0 if supported.
*/

#define _REENTRANT 1
#define _XOPEN_SOURCE 500
#include <pthread.h>

void* entry(void* arg) {
  return 0;
}

int main() {
  pthread_t id;
  if (!pthread_create(&id, 0, entry, 0)) {
   return 0;
  }
  return 1;
}
