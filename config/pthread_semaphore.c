/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  Runs if Posix semaphores are supported.
*/

#include <semaphore.h>

int main() {
  sem_t sem;
  int value;
  sem_init(&sem, 0, 1);
  sem_wait(&sem);
  sem_trywait(&sem);
  sem_post(&sem);
  sem_getvalue(&sem, &value);
  sem_destroy(&sem);
  return 0;
}
