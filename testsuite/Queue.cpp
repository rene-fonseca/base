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

using namespace dk::sdu::mip::base;

class QueueApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  QueueApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application(MESSAGE("Queue"), argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    fout << MESSAGE("Initializing queue") << ENDL;

    Queue<int> qi;

    fout << MESSAGE("Pushing values (1, 2, 3, 4, and 5)") << ENDL;
    qi.push(1);
    qi.push(2);
    qi.push(3);
    qi.push(4);
    qi.push(5);

    fout << MESSAGE("getSize() (5): ") << qi.getSize() << ENDL;
    fout << MESSAGE("isEmpty() (false): ") << qi.isEmpty() << ENDL;

    try {
      fout << MESSAGE("pop(): ") << qi.pop() << ENDL;
      fout << MESSAGE("pop(): ") << qi.pop() << ENDL;
      fout << MESSAGE("pop(): ") << qi.pop() << ENDL;
      fout << MESSAGE("pop(): ") << qi.pop() << ENDL;
      fout << MESSAGE("pop(): ") << qi.pop() << ENDL;
      fout << MESSAGE("pop(): ") << qi.pop() << ENDL;
    } catch (Exception& e) {
      ferr << TypeInfo::getTypename(e) << MESSAGE(": ") << e.getMessage() << ENDL;
    }
    fout << MESSAGE("InvalidNode exception is expected") << ENDL;

    fout << MESSAGE("getSize() (0): ") << qi.getSize() << ENDL;
    fout << MESSAGE("isEmpty() (true): ") << qi.isEmpty() << ENDL;
  }
};

STUB(QueueApplication);
