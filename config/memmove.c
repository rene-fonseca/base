/*
  This program links if memmove is provided by the c library.
*/

typedef unsigned int size_t;

#if defined(__cplusplus)
extern "C" {
#endif
  void* memmove(void*, const void*, size_t);
#if defined(__cplusplus)
}
#endif

int main() {
  memmove(0, 0, 0);
  return 0;
}
