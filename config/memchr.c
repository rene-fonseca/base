/*
  This program links if memchr is provided by the c library.
*/

typedef unsigned int size_t;

#if defined(__cplusplus)
extern "C" {
#endif
  void* memchr(const void*, int, size_t);
#if defined(__cplusplus)
}
#endif

int main() {
  memchr(0, 0, 0);
  return 0;
}
