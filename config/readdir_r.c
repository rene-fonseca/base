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
  This program compiles if readdir_r is provided by the c library.
*/

#include <dirent.h>

int main() {
  DIR* dir;
  struct dirent entry;
  struct dirent* result;
  
  if ((dir = opendir(".")) == 0) {
    return 1; // unable to test readdir_r
  }

  while (readdir_r(dir, &entry, &result)) {
    if (result == 0) {
      break;
    }
  }
					   
  closedir(dir); // ignore errors
  return 0;
}
