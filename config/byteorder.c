/*
  This program returns 0 if the byte order of the architecture is big
  endian otherwise 1 is returned.
*/

int main() {
  union {
    char c[sizeof(int)];
    int i;
  } u;
  u.i = 1;
  return u.c[0];
}
