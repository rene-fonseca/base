/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This program compiles if the compiler supports the restrict keyword.
*/

class MyClass {
private:

  char value;
public:

  void myFunction(char* restrict p, char* restirct q) {
  }
};

int main() {
  MyClass myClass;
  char* restrict p = 0;
  char* restrict q = 0;
  myClass.myFunction(p, q);
  return 0;
}
