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
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/concurrency/Process.h>
#include <base/Timer.h>

using namespace base;

class TimeApplication : public Application {
public:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  TimeApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("time"), numberOfArguments, arguments, environment) {
  }

  static String getTimeAsString(uint64 nanoseconds) throw() {
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
  
  void main() throw() {
    fout << Application::getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    const Array<String> arguments = getArguments();

    if (arguments.getSize() == 0) {
      fout << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" command") << ENDL;
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
    
    try {      
      Timer timer;
      timer.start();
      Process child = Process::execute(command);
      int exitCode = child.wait();
      timer.stop();
      setExitCode(exitCode);
      Process::Times times = child.getTimes();
      child.getTimes();
      fout << EOL
           << MESSAGE("Real: ") << setPrecision(3) << getTimeAsString(timer.getMicroseconds() * 1000ULL) << EOL
           << MESSAGE("User: ") << setPrecision(3) << getTimeAsString(times.user) << EOL
           << MESSAGE("System: ") << setPrecision(3) << getTimeAsString(times.system) << EOL
           << ENDL;
    } catch (Exception& e) {
      exceptionHandler(e);
    }
  }
  
};

int main(int argc, const char* argv[], const char* env[]) {
  TimeApplication application(argc, argv, env);
  try {
    application.main();
  } catch (Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch (...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
