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

class FailApplication;

class Worker : public Thread {
private:
  
  FailApplication* application = nullptr;
public:
  
  Worker(FailApplication* _app) : application(_app)
  {
  }
  
  void run();
};

class FailApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  String command;
  bool useThread = false;
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
      if (argument == "--thread") {
        useThread = true;
      } else if (!command) {
        command = argument;
      } else {
        ferr << Format::subst("Invalid argument '%1'.", argument) << ENDL;
        return false;
      }
    }
    if (!command) {
      ferr << "Command not specified (outofrange, rethrow, onlythrow, nullpointer, runtimeerror, outofmem, throwstring, throwstring2, terminate, resizeinit, resizedestroy, resizecopy, resizemove)." << ENDL;
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
      fout << "RETHROW" << ENDL;
      throw;
    }
  }
  
  void throwNothing()
  {
    throw; // TAG: this calls UnhandledException for win32 - add win32 handler
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

  class TestExceptionBase {
  public:
    
    TestExceptionBase() {
      fout << this << " " << "TestExceptionBase()" << ENDL;
    }

    ~TestExceptionBase() {
      fout << this << " " << "~TestExceptionBase(): " << (Exception::isUnwinding() ? "Unwind" : "Normal") << ENDL;
    }
  };

  template<int ID>
  class TestExceptionObject {
  public:
    
    TestExceptionObject() {
      fout << this << " " << "TestExceptionObject(): " << ID << ENDL;
    }

    ~TestExceptionObject() {
      fout << this << " " << "~TestExceptionObject(): " << ID << " " << (Exception::isUnwinding() ? "Unwind" : "Normal")<< ENDL;
    }
  };

  class TestExceptionThrow : public TestExceptionBase {
  public:
    
    TestExceptionObject<1> o1;
    
    TestExceptionThrow()
    {
      fout << this << " " << "TestExceptionThrow()" << ENDL;
      TestExceptionObject<2> o2;
      throw Exception("Constructor exception.");
    }

    ~TestExceptionThrow()
    {
      fout << this << " " << "~TestExceptionThrow(): " << (Exception::isUnwinding() ? "Unwind" : "Normal") << ENDL;
    }
  };

  class TestExceptionDestructThrow : public TestExceptionBase {
  public:
    
    TestExceptionObject<1> o1;
    
    TestExceptionDestructThrow()
    {
      fout << this << " " << "TestExceptionDestructThrow()" << ENDL;
    }

    ~TestExceptionDestructThrow() noexcept(false)
    {
      fout << this << " " << "~TestExceptionDestructThrow(): " << (Exception::isUnwinding() ? "Unwind" : "Normal") << ENDL;
      TestExceptionObject<2> o2;
      throw Exception("Destructor exception.");
    }
  };
  
  class TestExceptionDestructThrow2 : public TestExceptionBase {
  public:
    
    TestExceptionDestructThrow o1;
    
    TestExceptionDestructThrow2()
    {
      fout << this << " " << "TestExceptionDestructThrow2()" << ENDL;
    }

    ~TestExceptionDestructThrow2() noexcept(false)
    {
      fout << this << " " << "~TestExceptionDestructThrow2(): " << (Exception::isUnwinding() ? "Unwind" : "Normal") << ENDL;
      throw Exception("Destructor exception.");
    }
  };
  
  
  class MyClass {
  private:

    unsigned int id = 0;
  public:

    static unsigned int triggerId;
    static String cause;

    MyClass()
    {
      static unsigned int count = 0;
      id = count++;
      if ((id == triggerId) && (cause == "init")) {
        throw Exception("Throw on init.");
      }
    }

    MyClass(const MyClass& copy)
    {
      if ((id == triggerId) && (cause == "copy")) {
        throw Exception("Throw on copy construct.");
      }
    }

    MyClass(MyClass&& move)
    {
      if ((id == triggerId) && (cause == "move")) {
        throw Exception("Throw on move construct.");
      }
    }

    MyClass& operator=(const MyClass& assign)
    {
      if ((id == triggerId) && (cause == "copyassign")) {
        throw Exception("Throw on copy assign.");
      }
      return *this;
    }

    MyClass& operator=(MyClass&& assign)
    {
      if ((id == triggerId) && (cause == "moveassign")) {
        throw Exception("Throw on move assign.");
      }
      return *this;
    }

    ~MyClass()
    {
      if ((id == triggerId) && (cause == "destroy")) {
        throw Exception("Throw on destroy.");
      }
    }
  };

  void resizeArray(const char* cause)
  {
    MyClass::triggerId = 1234;
    MyClass::cause = cause;
    Array<MyClass> buffer;
    for (MemorySize i = 0; i < 4096; ++i) {
      fout << "#" << i << " " << FLUSH;
      buffer.append(MyClass());
    }
  }
    
  void doit()
  {
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
    } else if (command == "resizeinit") {
      resizeArray("init");
    } else if (command == "resizedestroy") {
      resizeArray("destroy");
    } else if (command == "resizecopy") {
      resizeArray("copy");
    } else if (command == "resizemove") {
      resizeArray("move");
    } else if (command == "resizecopyassign") {
      resizeArray("copyassign");
    } else if (command == "resizemoveassign") {
      resizeArray("moveassign");
    } else if (command == "construct") {
      TestExceptionThrow t1;
    } else if (command == "destruct") {
      try {
        TestExceptionDestructThrow t1;
      } catch (...) {
        fout << "CATCH" << ENDL;
      }
    } else if (command == "destruct2") {
      try {
        TestExceptionDestructThrow2 t1;
      } catch (...) {
        fout << "CATCH" << ENDL;
      }
    } else {
      setExitCode(123);
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
    
    if (useThread) {
      Worker worker(this);
      worker.start();
      worker.join();
    } else {
      doit();
    }
  }
};

void Worker::run()
{
  application->doit();
}

unsigned int FailApplication::MyClass::triggerId = 0;
String FailApplication::MyClass::cause;

APPLICATION_STUB(FailApplication);
