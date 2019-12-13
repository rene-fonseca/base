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
#include <base/string/StringOutputStream.h>
#include <base/concurrency/Process.h>
#include <base/Timer.h>

using namespace com::azure::dev::base;

class TimeApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  TimeApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) noexcept
    : Application("time", numberOfArguments, arguments, environment) {
  }

  static String getTimeAsString(uint64 nanoseconds) noexcept
  {
    uint64 seconds = nanoseconds/1000000000;
    uint32 minutes = seconds/60;
    uint32 hours = minutes/60;
    StringOutputStream stream;
    stream << setWidth(2) << ZEROPAD << hours << ':'
           << setWidth(2) << ZEROPAD << minutes%60 << ':'
           << setWidth(2) << ZEROPAD << seconds%60 << '.'
           << setWidth(3) << ZEROPAD << (nanoseconds/1000000)%1000
           << FLUSH;
    return stream.getString();
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    const Array<String> arguments = getArguments();

    if (arguments.getSize() == 0) {
      fout << "Usage: " << getFormalName() << " command" << ENDL;
      return;
    }
    
    String command;

    // merge arguments
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();

    String module = *enu.next();
    if (module.indexOf(' ') > 0) {
      command = "\"" + module + "\"";
    } else {
      command = module;
    }
    
    while (enu.hasNext()) {
      command += ' ';
      command += *enu.next();
    }
    
    Timer timer;
    Process child;
    int exitCode = 0;
    
    try {      
      timer.start();
      child = Process::execute(command);
      exitCode = child.wait();
      timer.stop();
    } catch (Process::ProcessException& e) {
      exceptionHandler(e);
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    setExitCode(exitCode);
    Process::Times times = child.getTimes();
    child.getTimes();
    fout << EOL
         << "Real: " << setPrecision(3)
         << getTimeAsString(timer.getMicroseconds() * 1000ULL) << EOL
         << "User: " << setPrecision(3)
         << getTimeAsString(times.user) << EOL
         << "System: " << setPrecision(3)
         << getTimeAsString(times.system) << EOL
         << ENDL;
  }
  
};

APPLICATION_STUB(TimeApplication);
