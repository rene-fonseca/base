/*
  This program links if the Large File System is supported by the c library.
*/

#define _LARGEFILE64_SOURCE 1

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  struct stat64 buffer;
  int a = stat64("", &buffer);
  off_t b = lseek64(0, 0, SEEK_SET);
  int c = ftruncate64(0, 0);
  return 0;
}
