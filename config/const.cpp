/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

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
