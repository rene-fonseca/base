#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>

int main()
{
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  printf("%ld.%06ld\n", spec.tv_sec, spec.tv_nsec/1000);
  return 0;
}
