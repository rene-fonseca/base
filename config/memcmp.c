/*
  This program links if memcmp is provided by the c library.
*/

typedef unsigned int size_t;

#if defined(__cplusplus)
extern "C" {
#endif
  int memcmp(const void*, const void*, size_t);
#if defined(__cplusplus)
}
#endif

int main() {
  memcmp(0, 0, 0);
  return 0;
}
