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
  This program compiles if the boolean type (bool) is supported.
*/

int main() {
  int myInteger;
  void* myPointer;

  // is type bool available
  bool myVariable;

  // are false and true defined
  myVariable = false;
  myVariable = true;

  myVariable = !myVariable;
  myVariable = true || false;
  myVariable = true && false;

  // are false and true compatible
  if (true == false) {
  }

  // check for implicit conversion to int
  myInteger = myVariable;

  // check for implicit conversion from int
  myVariable = myInteger;

  // check for implicit conversion from pointer
  myPointer = 0;
  myVariable = myPointer;

  return 0;
}
