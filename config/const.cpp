/*
  This program compiles if the compiler supports the const keyword.
*/

class MyClass {
private:

  char value;
public:

  const char* myFunction() const {
    return &value;
  }
};

int main() {
  MyClass myClass;
  const char* result = myClass.myFunction();
  return 0;
}
