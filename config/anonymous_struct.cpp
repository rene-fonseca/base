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
  This program compiles if the compiler supports anonymous structures.
*/

class MyClass {
public:

  void myMethod() {
    union {
      struct {
        int a;
        int b;
      };
    } c;
  }
};

int main() {
  MyClass object;
  object.myMethod();
  return 0;
}
