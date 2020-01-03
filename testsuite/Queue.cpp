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
#include <base/collection/Queue.h>
#include <base/string/FormatOutputStream.h>
#include <base/TypeInfo.h>

using namespace com::azure::dev::base;

class QueueApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  QueueApplication()
    : Application("Queue") {
  }
  
  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

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
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
      fout << "pop(): " << qi.pop() << ENDL;
#endif
    } catch (Exception& e) {
      ferr << TypeInfo::getTypename(e) << ": " << e.getMessage() << ENDL;
    }
    fout << "InvalidNode exception is expected" << ENDL;

    fout << "getSize() (0): " << qi.getSize() << ENDL;
    fout << "isEmpty() (true): " << qi.isEmpty() << ENDL;
  }
};

APPLICATION_STUB(QueueApplication);
