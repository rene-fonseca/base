/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/Stack.h>

using namespace base;

class StackApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  StackApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application(MESSAGE("Stack"), argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    fout << MESSAGE("Initializing Stack<String>") << ENDL;
    Stack<String> stack;
    
    fout << MESSAGE("Size: ") << stack.getSize() << ENDL;
    fout << MESSAGE("Content: ") << stack << ENDL;

    fout << MESSAGE("Pushing values onto stack") << ENDL;
    stack.push(MESSAGE("This"));
    stack.push(MESSAGE("is"));
    stack.push(MESSAGE("a"));
    stack.push(MESSAGE("message"));
    
    fout << MESSAGE("Size: ") << stack.getSize() << ENDL;
    fout << MESSAGE("Content: ") << stack << ENDL;

    while (!stack.isEmpty()) {
      fout << MESSAGE("Popped value: ") << stack.pop() << ENDL;
    }

    fout << MESSAGE("Size: ") << stack.getSize() << ENDL;
    fout << MESSAGE("Content: ") << stack << ENDL;

    fout << MESSAGE("Pop on empty stack (exception OutOfRange expected)") << ENDL;
    try {
      stack.pop();
    } catch (OutOfRange& e) {
      fout << MESSAGE("Exception caught: ") << e << ENDL;
    }
    
    fout << MESSAGE("Removing all values") << ENDL;
    stack.removeAll();
    
    fout << MESSAGE("Size: ") << stack.getSize() << ENDL;
    fout << MESSAGE("Content: ") << stack << ENDL;
  }
};

STUB(StackApplication);
