/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/concurrency/Thread.h>
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
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/sound/SoundDevice.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <stdlib.h>

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
    : thread(static_cast<Thread*>(0)), threadLocal(&thread) { // no parent for main thread
  }
};

namespace internal {

  class ProcessInitialization {
  private:
    
    ProcessPreinitialization processPreinitialization;
  public:
    
    ProcessInitialization() throw() {}
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
      char message[sizeof("0x1234567812345678 allocations have not been releaced for DebugDynamicMemory")];
      char* dest = message;
      *dest++ = '0';
      *dest++ = 'x';
      
      unsigned long value = DebugDynamicMemory::currentAllocations;
      for (unsigned int i = 0; i < (sizeof(unsigned long) * 2); ++i) {
        dest[sizeof(unsigned long)*2 - 1 - i] = ASCIITraits::valueToDigit(value & 0x0f);
        value >>= 4; // bits per digit
      }
      dest += sizeof(unsigned long) * 2;
      
      copy<char>(dest, " allocations have not been releaced for DebugDynamicMemory", sizeof(" allocations have not been releaced for DebugDynamicMemory") - 1);
      dest += sizeof(" allocations have not been releaced for DebugDynamicMemory") - 1;
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
const WideString WideString::DEFAULT_STRING(MESSAGE(""));

namespace internal {
  
  void terminationExceptionHandler() throw() {
    Trace::message("Exception was raised during application initialization or cleanup.");
    SystemLogger::write(SystemLogger::ERROR, MESSAGE("Exception was raised during application initialization or cleanup."));
    // ferr has not been initialized
    exit(Application::EXIT_CODE_INITIALIZATION);
  }

  void unexpectedExceptionHandler() throw() {
    StringOutputStream stream;
    try {
      throw;
    } catch(Exception& e) {
      stream << MESSAGE("Internal error: exception '") << TypeInfo::getTypename(e) << MESSAGE("' was raised");
      if (e.getType().isInitialized()) {
        stream << MESSAGE(" by '") << TypeInfo::getTypename(e.getType()) << '\'';
      }
      if (e.getMessage()) {
        stream << MESSAGE(" with message '") << e.getMessage() << '\'';
      }
      stream << MESSAGE(" in violation with exception specification during application initialization or cleanup.") << FLUSH;
    } catch(...) {
      stream << MESSAGE("Internal error: unsupported exception was raised in violation with exception specification during application initialization or cleanup.") << FLUSH;
    }
    Trace::message(stream.getString().getElements());
    SystemLogger::write(SystemLogger::ERROR, stream.getString().getElements());
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

Handle* FileDescriptor::Descriptor::invalid; // uninitialized
Handle* File::FileHandle::invalid; // uninitialized
Handle* Pipe::PipeHandle::invalid; // uninitialized
Socket::SocketImpl* Socket::SocketImpl::invalid; // uninitialized

class Initialization {
private:

  Handle invalidHandle;
  Socket::SocketImpl invalidSocket;
public:

  Initialization() throw() : invalidSocket(OperatingSystem::INVALID_HANDLE) {
    Random::randomize(); // randomize global random number generator

    // having a global invalid handle safes us from allocating/deallocating many handles
    ReferenceCountedObjectPointerImpl(invalidHandle).addReference(); // prevent destruction of object
    ReferenceCountedObjectPointerImpl(invalidSocket).addReference(); // prevent destruction of object
    FileDescriptor::Descriptor::invalid = &invalidHandle;
    File::FileHandle::invalid = &invalidHandle;
    Pipe::PipeHandle::invalid = &invalidHandle;
    Socket::SocketImpl::invalid = &invalidSocket;
  }
};

Initialization initialization;

FileDescriptorInputStream standardInputStream(FileDescriptor::getStandardInput());
FormatInputStream fin(standardInputStream);

FileDescriptorOutputStream standardOutputStream(FileDescriptor::getStandardOutput());
FormatOutputStream fout(standardOutputStream);

FileDescriptorOutputStream standardErrorStream(FileDescriptor::getStandardError());
FormatOutputStream ferr(standardErrorStream);

SoundDevice SoundDevice::soundDevice;

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
