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
