/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program compiles/links/runs if the compiler supports the nanosleep.
*/

#include <base/platforms/symbols.h>
#include <base/platforms/platform.h>
#include <time.h>

int main() {
  struct timespec interval;
  interval.tv_sec = 0;
  interval.tv_nsec = 12345;
  int result = nanosleep(&interval, &interval);
  return 0;
}
