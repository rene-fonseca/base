/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/string/StringOutputStream.h>
#include <base/ProcessPreinitialization.h>
#include <base/SystemLogger.h>
#include <base/Trace.h>
#include <base/Application.h>
#include <base/TypeInfo.h>
#include <base/mem/DebugDynamicMemory.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/Process.h>
#include <base/string/String.h>
#include <base/string/WideString.h>
#include <base/mathematics/Random.h>
#include <base/io/Handle.h>
#include <base/io/FileDescriptor.h>
#include <base/io/FileDescriptorInputStream.h>
#include <base/io/FileDescriptorOutputStream.h>
#include <base/io/File.h>
#include <base/io/Pipe.h>
#include <base/net/Socket.h>
#include <base/string/Locale.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/sound/SoundDevice.h>
#include <base/Literal.h>
#include <stdlib.h>

int _DUMMY_SYMBOL = 0;

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/*
  The order of initialization is very significant. fin, fout, and ferr are the
  only variables with are located directly within the base namespace. All other
  variables are located in the internal namespace or their associated classes.
*/

class ThreadImpl {
private:
  
  // the main thread
  Thread thread;
  // thread specific information
  Thread::ThreadLocal threadLocal;
public:
  
  ThreadImpl() throw()
    : thread(static_cast<Thread*>(nullptr)),
      threadLocal(&thread) { // no parent for main thread
  }
};

namespace internal {

  class ProcessInitialization {
  private:
    
    ProcessPreinitialization processPreinitialization;
  public:
    
    ProcessInitialization() throw() {
    }
  };
  
  ProcessInitialization processInitialization;
}; // end of namespace - internal

SpinLock DebugDynamicMemory::spinLock;
unsigned long DebugDynamicMemory::currentAllocations(0);

class DebugDynamicMemoryImpl {
public:
    
  DebugDynamicMemoryImpl() throw() {
  }

  ~DebugDynamicMemoryImpl() throw() {
    if (DebugDynamicMemory::currentAllocations > 0) {
      char message[sizeof("0x1234567812345678 allocations have not been released for DebugDynamicMemory")];
      char* dest = message;
      *dest++ = '0';
      *dest++ = 'x';
      
      unsigned long value = DebugDynamicMemory::currentAllocations;
      for (unsigned int i = 0; i < (sizeof(unsigned long) * 2); ++i) {
        dest[sizeof(unsigned long)*2 - 1 - i] =
          ASCIITraits::valueToDigit(value & 0x0f);
        value >>= 4; // bits per digit
      }
      dest += sizeof(unsigned long) * 2;

      static const Literal
        literal(" allocations have not been released for DebugDynamicMemory");
      copy<char>(dest, literal.getValue(), literal.getLength());
      dest += literal.getLength();
      *dest = 0; // terminate string
      Trace::message(message);
    }
  }
};

namespace internal {

  DebugDynamicMemoryImpl debugDynamicMemoryImpl;
};

Application* Application::application(0); // initialize application as uninitialized
const String String::DEFAULT_STRING(MESSAGE(""));
const WideString WideString::DEFAULT_STRING(WIDEMESSAGE(""));

namespace internal {
  
  void terminationExceptionHandler() throw() {
    Trace::message("Exception was raised during application initialization or cleanup.");
    SystemLogger::write(
      SystemLogger::ERROR,
      "Exception was raised during application initialization or cleanup."
    );
    // ferr has not been initialized
    exit(Application::EXIT_CODE_INITIALIZATION);
  }

  void unexpectedExceptionHandler() throw() {
    StringOutputStream stream;
    try {
      throw;
    } catch (Exception& e) {
      stream << "Internal error: exception '"
             << TypeInfo::getTypename(e) << "' was raised";
      if (e.getType().isInitialized()) {
        stream << " by '" << TypeInfo::getTypename(e.getType()) << '\'';
      }
      const char* message = e.getMessage();
      unsigned int cause = e.getCause();
      if (message || (cause != PrimitiveTraits<unsigned int>::MAXIMUM)) {
        stream << " with";
      }
      if (message) {
        stream << " message '" << NativeString(message) << '\'';
      }
      if (message && (cause != PrimitiveTraits<unsigned int>::MAXIMUM)) {
        stream << " and";
      }
      if (cause != PrimitiveTraits<unsigned int>::MAXIMUM) {
        stream << " cause " << HEX << setWidth(10) << ZEROPAD << cause;
      }
      stream << " in violation with exception specification during application initialization or cleanup." << FLUSH;
    } catch (...) {
      stream << "Internal error: unsupported exception was raised in violation with exception specification during application initialization or cleanup." << FLUSH;
    }
    Trace::message(stream.getString().getElements());
    SystemLogger::write(SystemLogger::ERROR, stream.getString());
    exit(Application::EXIT_CODE_INITIALIZATION);
  }
  
  class ExceptionHandlers {
  public:
    
    ExceptionHandlers() throw() {
      // install exception handlers
      std::set_terminate(terminationExceptionHandler);
      std::set_unexpected(unexpectedExceptionHandler);
    }
  };

  ExceptionHandlers excepionHandlers;
}; // end of namespace - internal

namespace internal {
  
  // setup main thread object
  ThreadImpl threadImpl; // use this variable through 'threadLocal'
}; // end of namespace internal

SpinLock Random::spinLock;

Handle* FileDescriptor::Descriptor::invalid = nullptr; // uninitialized
Handle* Process::ProcessHandle::invalid = nullptr; // uninitialized
Handle* File::FileHandle::invalid = nullptr; // uninitialized
Handle* Pipe::PipeHandle::invalid = nullptr; // uninitialized
Socket::SocketImpl* Socket::SocketImpl::invalid = nullptr; // uninitialized

class Initialization {
private:

  Handle invalidHandle;
  Socket::SocketImpl invalidSocket;
public:

  Initialization() throw()
    : invalidSocket(OperatingSystem::INVALID_HANDLE, Socket::IPV4, Socket::STREAM) {
    Random::randomize(); // randomize global random number generator

    // having a global invalid handle safes us from allocating/deallocating many handles
    ReferenceCountedObject::ReferenceImpl(invalidHandle).addReference(); // prevent destruction of object
    ReferenceCountedObject::ReferenceImpl(invalidSocket).addReference(); // prevent destruction of object
    FileDescriptor::Descriptor::invalid = &invalidHandle;
    Process::ProcessHandle::invalid = &invalidHandle;
    File::FileHandle::invalid = &invalidHandle;
    Pipe::PipeHandle::invalid = &invalidHandle;
    Socket::SocketImpl::invalid = &invalidSocket;
  }
};

Initialization initialization;
const Locale Locale::POSIX;

FileDescriptorInputStream standardInputStream(
  FileDescriptor::getStandardInput()
);
FormatInputStream _DK_SDU_MIP__BASE__API fin(standardInputStream);

FileDescriptorOutputStream standardOutputStream(
  FileDescriptor::getStandardOutput()
);
_DK_SDU_MIP__BASE__API FormatOutputStream fout(standardOutputStream);

FileDescriptorOutputStream standardErrorStream(
  FileDescriptor::getStandardError()
);
FormatOutputStream _DK_SDU_MIP__BASE__API ferr(standardErrorStream);

SoundDevice SoundDevice::soundDevice;

void moduleEntry() {
}

void moduleCleanUp() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
