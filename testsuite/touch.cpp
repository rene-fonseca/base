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
#include <base/string/FormatOutputStream.h>
#include <base/io/File.h>

using namespace com::azure::dev::base;

class TouchApplication : public Application {
public:

  // TAG: add support for setting version
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  TouchApplication()
    : Application("touch")
  {
  }

  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    const Array<String> arguments = getArguments();

    if (arguments.getSize() == 0) {
      fout << "Usage: " << getFormalName() << " [OPTIONS] [FILE]" << ENDL;
      return;
    }
    
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();

    String module = *enu.next();
    // -a --access
    // -m --modification
    // -c --no-create
    // -r --reference=FILE
    // -t --time=format?
    // --help
    // --version
    
#if 0
    // DateTime time;
    File file(arguments[0]);
    file.setCreationTime();
    file.setModificationTime;
#endif
    
    try {
    } catch (Exception& e) {
      exceptionHandler(e);
    }
  }
};

APPLICATION_STUB(TouchApplication);
