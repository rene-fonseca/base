/*
  Compiles and runs if read-write lock is supported by the pthread library.
*/

#define _XOPEN_SOURCE 500
#include <pthread.h>

int main() {
  pthread_rwlockattr_t attributes;
  pthread_rwlock_t lock;
  pthread_rwlockattr_init(&attributes);
  pthread_rwlockattr_setpshared(&attributes, PTHREAD_PROCESS_PRIVATE);
  pthread_rwlock_init(&lock, &attributes);
  pthread_rwlockattr_destroy(&attributes);

  pthread_rwlock_wrlock(&lock);
  pthread_rwlock_unlock(&lock);

  pthread_rwlock_trywrlock(&lock);
  pthread_rwlock_unlock(&lock);

  pthread_rwlock_rdlock(&lock);
  pthread_rwlock_unlock(&lock);

  pthread_rwlock_tryrdlock(&lock);
  pthread_rwlock_unlock(&lock);

  pthread_rwlock_destroy(&lock);
  return 0;
}
