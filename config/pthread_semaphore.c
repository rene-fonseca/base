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
