/*
  This program links if memset is provided by the c library.
*/

typedef unsigned int size_t;

#if defined(__cplusplus)
extern "C" {
#endif
  void* memset(void*, int, size_t);
#if defined(__cplusplus)
}
#endif

int main() {
  memset(0, 0, 0);
  return 0;
}
