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
  This program compiles if the compiler supports exception specification.
*/

class MyException {
public:

  MyException() throw() {
  }
};

class MyClass {
public:

  MyClass() throw() {
  }
  
  static void myStaticMethod() throw(MyException) {
    throw MyException();
  }

  void myMethod() throw(MyException) {
    throw MyException();
  }
};

int main() {
  MyClass object;
  try {
    object.myMethod();
  } catch (MyException&) {
  }
  return 0;
}
