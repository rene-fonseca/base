/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/OutOfRange.h>

using namespace base;

void myInnerFunction() throw(OutOfRange) {
  throw OutOfRange("This is a message");
}

void myOuterFunction() throw() {
  if (true) {
    myInnerFunction();
    // unexpected exception expected
  } else {
    try {
      myInnerFunction();
    } catch(const Exception& e) {
      fout << "Handled exception" << ENDL;
    }
  }
}

int main(int argc, const char* argv[], const char* envp[]) {
  fout << "Testing exception handling..." << EOL << ENDL;

  Application app("Exception", argc, argv, envp);
  try {
    myOuterFunction();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::EXIT_CODE_NORMAL;
}
