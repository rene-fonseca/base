/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/Queue.h>
#include <base/string/FormatOutputStream.h>
#include <base/TypeInfo.h>

using namespace base;

class QueueApplication : public Application {
public:

  QueueApplication(int argc, const char* argv[], const char* env[]) : Application(MESSAGE("Queue"), argc, argv, env) {
  }
  
  void main() throw() {
    fout << "Testing implementation of the Queue class" << ENDL;

    fout << "Initializing queue" << ENDL;

    Queue<int> qi;

    fout << "Pushing values (1, 2, 3, 4, and 5)" << ENDL;
    qi.push(1);
    qi.push(2);
    qi.push(3);
    qi.push(4);
    qi.push(5);

    fout << "getSize() (5): " << qi.getSize() << ENDL;
    fout << "isEmpty() (false): " << qi.isEmpty() << ENDL;

    try {
      fout << "pop(): " << qi.pop() << ENDL;
      fout << "pop(): " << qi.pop() << ENDL;
      fout << "pop(): " << qi.pop() << ENDL;
      fout << "pop(): " << qi.pop() << ENDL;
      fout << "pop(): " << qi.pop() << ENDL;
      fout << "pop(): " << qi.pop() << ENDL;
    } catch(Exception& e) {
      ferr << TypeInfo::getTypename(e) << ": " << e.getMessage() << ENDL;
    }
    fout << "InvalidNode exception is expected" << ENDL;

    fout << "getSize() (0): " << qi.getSize() << ENDL;
    fout << "isEmpty() (true): " << qi.isEmpty() << ENDL;
  }
};

int main(int argc, const char* argv[], const char* env[]) {
  QueueApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
