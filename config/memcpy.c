/*
  This program links if memcpy is provided by the c library.
*/

typedef unsigned int size_t;

#if defined(__cplusplus)
extern "C" {
#endif
  void* memcpy(void*, const void*, size_t);
#if defined(__cplusplus)
}
#endif

int main() {
  memcpy(0, 0, 0);
  return 0;
}
