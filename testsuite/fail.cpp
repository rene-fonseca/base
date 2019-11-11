/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Timer.h>
#include <base/Primitives.h>
#include <base/LongInteger.h>
#include <base/io/FileOutputStream.h>
#include <base/concurrency/Thread.h>
#include <base/net/HTTPSRequest.h>
#include <base/net/Url.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/string/Format.h>
#include <base/objectmodel/JSON.h>

using namespace com::azure::dev::base;

class FailApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  String command;
public:
  
  FailApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("fail", numberOfArguments, arguments, environment)
  {
  }
  
  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = *enu.next();
      if (command) {
        ferr << Format::subst("Invalid argument '%1'.", argument) << ENDL;
        return false;
      }
      command = argument;
    }
    if (!command) {
      ferr << "Command not specified (outofrange, rethrow, onlythrow, nullpointer, runtimeerror, outofmem, throwstring, throwstring2, terminate)." << ENDL;
      return false;
    }
    return true;
  }

  void throwOutOfRange()
  {
    throw OutOfRange("Testing throw of out of range exception which must get caught by application stub.");
  }

  void rethrowOutOfRange()
  {
    try {
      throwOutOfRange();
    } catch (Exception&) {
      throw;
    }
  }
  
  void throwNothing()
  {
    throw;
  }
  
  void throwNullPointer() noexcept
  {
    Reference<ReferenceCountedObject> dummy;
    dummy->getNumberOfReferences_INTERNAL(); // triggers std::terminate() due to noexcept
  }
  
  void throwStdRuntimeError()
  {
    throw std::runtime_error("Testing throw of std::exception which must get caught by application stub.");
  }

  void throwString()
  {
    throw "Testing throw of leteral which must get caught by application stub.";
  }

  void throwString2() noexcept
  {
    throwString();
  }
  
  void dumpStack(unsigned int count = 10)
  {
    if (count == 0) {
      StackFrame::dump();
    } else {
      dumpStack(count - 1);
    }
  }

  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    if (!parseArguments()) {
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
    
    if (command == "outofrange") {
      throwOutOfRange();
    } else if (command == "rethrow") {
      rethrowOutOfRange();
    } else if (command == "onlythrow") {
      throwNothing();
    } else if (command == "nullpointer") {
      throwNullPointer();
    } else if (command == "runtimeerror") {
      throwStdRuntimeError();
    } else if (command == "throwstring") {
      throwString();
    } else if (command == "throwstring2") {
      throwString2();
    } else if (command == "outofmem") {
      Allocator<uint8> buffer;
      MemorySize size = 0;
      --size;
      size /= 2;
      buffer.setSize(size);
    } else if (command == "terminate") {
      std::terminate();
    } else if (command == "dumpstack") {
      dumpStack();
    } else {
      setExitCode(123);
    }
  }
};

APPLICATION_STUB(FailApplication);
