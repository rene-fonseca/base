/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/Stack.h>

using namespace com::azure::dev::base;

class StackApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  StackApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application("Stack", argc, argv, env) {
  }
  
  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    fout << "Initializing Stack<String>" << ENDL;
    Stack<String> stack;
    
    fout << "Size: " << stack.getSize() << ENDL;
    fout << "Content: " << stack << ENDL;

    fout << "Pushing values onto stack" << ENDL;
    stack.push("This");
    stack.push("is");
    stack.push("a");
    stack.push("message");
    
    fout << "Size: " << stack.getSize() << ENDL;
    fout << "Content: " << stack << ENDL;

    while (!stack.isEmpty()) {
      fout << "Popped value: " << stack.pop() << ENDL;
    }

    fout << "Size: " << stack.getSize() << ENDL;
    fout << "Content: " << stack << ENDL;

    fout << "Pop on empty stack (exception OutOfRange expected)" << ENDL;
    try {
      stack.pop();
    } catch (OutOfRange& e) {
      fout << "Exception caught: " << e << ENDL;
    }
    
    fout << "Removing all values" << ENDL;
    stack.removeAll();
    
    fout << "Size: " << stack.getSize() << ENDL;
    fout << "Content: " << stack << ENDL;
  }
};

APPLICATION_STUB(StackApplication);
