/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) // annoying dependency
#  pragma warning(disable : 4073)
#  pragma init_seg(lib)
#endif

#include <base/platforms/features.h>
#include <base/initialization.h>
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
#include <base/math/Random.h>
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
#include <base/Profiler.h>
#include <base/UnitTest.h>
#include <base/Module.h>
#include <stdlib.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

namespace _impl {

  bool initializing = true;
  bool destructing = false;

  class RuntimeState {
  public:

    RuntimeState()
    {
      _impl::initializing = false;
    }

    ~RuntimeState()
    {
      _impl::destructing = true;
    }
  };
};

/** The thread object associated with context. */
ThreadKey<ThreadLocalContext> Thread::threadLocalContext; // thread object

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
  
  ThreadImpl() noexcept
    : thread(static_cast<Thread*>(nullptr)),
      threadLocal(&thread) // no parent for main thread
  {
    Thread::setThreadName("main");
  }
};

#if 0
class TrackProgress {
private:
    
  static unsigned int count;
  const char* text = nullptr;
  unsigned int level = 0;
public:
    
  TrackProgress(const char* _text = "Progress") noexcept : text(_text)
  {
    ++count;
    level = count;
    printf("TRACK: %s [%d]\n", text, level);
  }

  ~TrackProgress() noexcept
  {
    printf("TRACK: %s [%d]\n", text, level);
  }
};

unsigned int TrackProgress::count = 0;

#define TRACK_PROGRESS(message) \
  TrackProgress _CONCATENATE(_tp_, __LINE__)(message)

#endif

namespace internal {

  class ProcessInitialization {
  private:
    
    ProcessPreinitialization processPreinitialization;
  public:
    
    ProcessInitialization()
    {
    }
  };

  ProcessInitialization processInitialization;
}; // end of namespace - internal

SpinLock DebugDynamicMemory::spinLock;
PreferredAtomicCounter DebugDynamicMemory::currentAllocations;

class DebugDynamicMemoryImpl {
public:
    
  DebugDynamicMemoryImpl() noexcept
  {
  }

  ~DebugDynamicMemoryImpl() noexcept
  {
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

      static const Literal literal(" allocations have not been released for DebugDynamicMemory");
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

MutualExclusion Application::lock;

Application* Application::application(nullptr); // initialize application as uninitialized
String::Default forceDefaultString;
const String String::DEFAULT_STRING(forceDefaultString);
const WideString WideString::DEFAULT_STRING(forceDefaultString);

namespace internal {
  
  void terminationExceptionHandler() noexcept
  {
    Trace::message("Exception was raised during application initialization or cleanup.");
    SystemLogger::write(
      SystemLogger::ERROR,
      "Exception was raised during application initialization or cleanup."
    );
    // ferr has not been initialized
    exit(Application::EXIT_CODE_INITIALIZATION);
  }

  void unexpectedExceptionHandler() noexcept
  {
    StringOutputStream stream;
    try {
      throw;
    } catch (Exception& e) {
      stream << "Internal error: Exception '"
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
      stream << "Internal error: Unsupported exception was raised in violation with exception specification during application initialization or cleanup." << FLUSH;
    }
    Trace::message(stream.getString().getElements());
    SystemLogger::write(SystemLogger::ERROR, stream.getString());
    ferr << stream.getString() << ENDL;
    exit(Application::EXIT_CODE_INITIALIZATION);
  }
  
  class ExceptionHandlers {
  public:
    
    ExceptionHandlers() noexcept
    {
      // install exception handlers
      std::set_terminate(terminationExceptionHandler);
      std::set_unexpected(unexpectedExceptionHandler);
    }
  };

  ExceptionHandlers excepionHandlers;
}; // end of namespace - internal

namespace internal {

  void initializeTimer();
  const bool _initializeTimer = (static_cast<void>(initializeTimer()), true);

  // setup main thread object
  ThreadImpl threadImpl; // use this variable through 'threadLocal'
}; // end of namespace internal

SpinLock RandomLegacy::spinLock;
UnitTestManager UnitTestManager::unitTestManager;
ModuleManager ModuleManager::moduleManager;

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

  Initialization() noexcept
    : invalidSocket(OperatingSystem::INVALID_HANDLE, Socket::IPV4, Socket::STREAM)
  {
    RandomLegacy::randomize(); // randomize global random number generator

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

const FormatOutputStream::Context FormatOutputStream::DEFAULT_CONTEXT = FormatOutputStream::Context();

FileDescriptorInputStream standardInputStream(
  FileDescriptor::getStandardInput()
);
FormatInputStream _COM_AZURE_DEV__BASE__API fin(standardInputStream);

FileDescriptorOutputStream standardOutputStream(
  FileDescriptor::getStandardOutput()
);
FormatOutputStream _COM_AZURE_DEV__BASE__API fout(standardOutputStream);

FileDescriptorOutputStream standardErrorStream(
  FileDescriptor::getStandardError()
);
FormatOutputStream _COM_AZURE_DEV__BASE__API ferr(standardErrorStream);

SoundDevice SoundDevice::soundDevice;

bool Profiler::enabled = 0;
Profiler::ProfilerImpl Profiler::profiler;

Application::Stub::Stub()
{
  BASSERT(_impl::initializing && !_impl::destructing);
  _impl::initializing = false;
}

Application::Stub::~Stub()
{
  BASSERT(!_impl::initializing && !_impl::destructing);
  _impl::destructing = true;
}

bool Runtime::isGlobalInitialization() noexcept
{
  return _impl::initializing; // TAG: better to ask compiler ABI
}

bool Runtime::isGlobalDestruction() noexcept
{
  return _impl::destructing; // TAG: better to ask compiler ABI
}

bool Runtime::isGlobalStateInGoodCondition() noexcept
{
  return !Runtime::isGlobalInitialization() && !Runtime::isGlobalDestruction();
}

void moduleEntry()
{
}

void moduleCleanUp()
{
}

namespace cleanup {

  SpinLock lock;
  Cleanup::CleanupFunction functions[32] = {0}; // ATTENTION: fix size array so we can only use internally to Base
  unsigned int count = 0;

  bool registerCleanup(Cleanup::CleanupFunction function) noexcept
  {
    SpinLock::Sync _sync(lock);
    if (function) {
      if (INLINE_ASSERT(count < getArraySize(functions))) {
        functions[count++] = function;
        return true;
      }
    }
    return false;
  }

  void cleanupAll()
  {
    // clean up in reverse order!
    while (count > 0) {
      --count;
      functions[count]();
      functions[count] = nullptr;
    }
  }
}

bool Cleanup::registerCleanup(CleanupFunction function) noexcept
{
  return cleanup::registerCleanup(function);
}

class CleanupImpl {
public:

  ~CleanupImpl()
  {
    cleanup::cleanupAll();
  }
};

CleanupImpl _cleanup; // keep at end

// _impl::RuntimeState runtimeState; // need this

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
