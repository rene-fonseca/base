/*
  This program compiles if the compiler supports anonymous structures.
*/

int main() {
  union {
    struct {
      int a;
      int b;
    };
  } c;
  return 0;
}
