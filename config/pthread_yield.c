#if !defined(_GNU_SOURCE)
  #define _GNU_SOURCE
#endif

#include <pthread.h>

int main() {
  pthread_yield();
  return 0;
}
