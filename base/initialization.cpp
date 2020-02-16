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
#include <base/NotImplemented.h>
#include <base/NotSupported.h>
#include <base/UnexpectedFailure.h>
#include <iostream>
#include <stdlib.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__TESTS)
#define DEPENDENCY(ID) _COM_AZURE_DEV__BASE__ADD_DEPENDENCY(ID);

// ./testsuite/Debug/unittesting --externals | grep TEST | sed -e "s/TEST [^ ]*\//DEPENDENCY(/g" | sed -e "s/ .*/);/g" | sort

DEPENDENCY(Adler32);
DEPENDENCY(AdvancedEncryptionStandard);
DEPENDENCY(Allocator);
DEPENDENCY(AnyValue);
DEPENDENCY(Array);
DEPENDENCY(ArrayMap);
DEPENDENCY(Association);
DEPENDENCY(AtomicCounter);
DEPENDENCY(Base64);
DEPENDENCY(BitSet);
DEPENDENCY(Buffer);
DEPENDENCY(CPP);
DEPENDENCY(CRC32);
DEPENDENCY(Complex);
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
DEPENDENCY(DOMImplementation);
#endif
DEPENDENCY(DynamicLinker);
DEPENDENCY(DynamicObject);
DEPENDENCY(Event);
DEPENDENCY(Exception);
DEPENDENCY(File);
DEPENDENCY(FloatingPoint);
DEPENDENCY(Format);
DEPENDENCY(FormatOutputStream);
DEPENDENCY(Guid);
DEPENDENCY(HTTPSRequest);
DEPENDENCY(HashSet);
DEPENDENCY(HashTable);
DEPENDENCY(Heap);
DEPENDENCY(InetAddress);
DEPENDENCY(InetEndPoint);
DEPENDENCY(InstanceCounter);
DEPENDENCY(JSON);
DEPENDENCY(LargeInteger);
DEPENDENCY(List);
DEPENDENCY(MD5Sum);
DEPENDENCY(Map);
DEPENDENCY(Math);
DEPENDENCY(MutualExclusion);
DEPENDENCY(ObjectModel);
DEPENDENCY(Once);
DEPENDENCY(Pair);
DEPENDENCY(Parser);
DEPENDENCY(Pipe);
DEPENDENCY(Posix);
DEPENDENCY(PriorityQueue);
DEPENDENCY(Process);
DEPENDENCY(Random);
DEPENDENCY(ReadWriteLock);
DEPENDENCY(RecursiveMutualExclusion);
DEPENDENCY(RecursiveSpinLock);
DEPENDENCY(Reference);
DEPENDENCY(ReferenceCounter);
DEPENDENCY(RegExp);
DEPENDENCY(SHA1);
DEPENDENCY(SHA256);
DEPENDENCY(SHA384);
DEPENDENCY(SHA512);
DEPENDENCY(Semaphore);
DEPENDENCY(Set);
DEPENDENCY(Socket);
DEPENDENCY(SpinLock);
DEPENDENCY(Stack);
DEPENDENCY(String);
DEPENDENCY(Thread);
DEPENDENCY(ThreadKey);
DEPENDENCY(Timer);
DEPENDENCY(Type);
DEPENDENCY(Unicode);
DEPENDENCY(Url);
DEPENDENCY(Urn);
DEPENDENCY(Vector2D);
DEPENDENCY(Vector3D);
DEPENDENCY(WideString);
DEPENDENCY(YAML);
#endif

#if 0
Print _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(print)("PRINT: HERE " __FILE__ ":" _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__));
#endif

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

namespace _impl {

  bool initializing = true;
  bool destructing = false;
  bool runtimeEnvironment = false;

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
    GlobalPrint::printf("TRACK: %s [%d]\n", text, level);
  }

  ~TrackProgress() noexcept
  {
    GlobalPrint::printf("TRACK: %s [%d]\n", text, level);
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
const String String::DEFAULT_STRING = String::Default();
const WideString WideString::DEFAULT_STRING = WideString::Default();

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
    Trace::message(stream.getString().native());
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

class Initialization {
private:

  R<Handle> invalidHandle;
public:

  Initialization() noexcept
  {
    RandomLegacy::randomize(); // randomize global random number generator

    // having a global invalid handle safes us from allocating/deallocating many handles
    invalidHandle = new Handle();
    FileDescriptor::Descriptor::invalid = invalidHandle.getValue();
    Process::ProcessHandle::invalid = invalidHandle.getValue();
  }

  ~Initialization()
  {
    FileDescriptor::Descriptor::invalid = nullptr;
    Process::ProcessHandle::invalid = nullptr;
  }
};

Initialization initialization;
const Locale Locale::POSIX;

const FormatOutputStream::Context FormatOutputStream::DEFAULT_CONTEXT = FormatOutputStream::Context();

extern bool isRunningXeusCling();

class OutputStream2OStream : public OutputStream {
private:

  std::ostream* os = nullptr;
public:
  
  OutputStream2OStream(std::ostream* _os) noexcept
    : os(_os)
  {
  }

  void close()
  {
    // never close
  }

  void flush()
  {
    os->flush();
  }

  unsigned int write(const uint8* _buffer, unsigned int _size, bool _nonblocking)
  {
#if 0 // old data is cached somewhere and output again
    *os << reinterpret_cast<const char*>(_buffer);
    os->flush();
#endif
#if 0 // works
    for (unsigned int i = 0; i < _size; ++i) {
      os->put(_buffer[i]);
    }
#endif
#if 1 // works
    if (std::streambuf* buffer = os->rdbuf()) {
      const uint8* src = _buffer;
      const uint8* end = _buffer + _size;
      while (src != end) {
        buffer->sputc(*src++);
      }
      // return buffer->sputn(reinterpret_cast<const char*>(buffer), _size); // not working
    }
#endif
    return _size;
  }
};

// used for xeus/cling to redirect output to browser
OutputStream2OStream coutOutputStream(&std::cout);
OutputStream2OStream cerrOutputStream(&std::cerr);

FileDescriptorInputStream standardInputStream(
  FileDescriptor::getStandardInput()
);
FormatInputStream _COM_AZURE_DEV__BASE__API fin(standardInputStream);

FileDescriptorOutputStream standardOutputStream(
  FileDescriptor::getStandardOutput()
);
FormatOutputStream _COM_AZURE_DEV__BASE__API fout(
  isRunningXeusCling() ? static_cast<OutputStream&>(coutOutputStream) : standardOutputStream
);

FileDescriptorOutputStream standardErrorStream(
  FileDescriptor::getStandardError()
);
FormatOutputStream _COM_AZURE_DEV__BASE__API ferr(
  isRunningXeusCling() ? static_cast<OutputStream&>(cerrOutputStream) : standardErrorStream
);

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

void Runtime::setRuntimeEnvironment(const char* _id) noexcept
{
  _impl::runtimeEnvironment = _id != nullptr;
}

bool Runtime::getRuntimeEnvironment() noexcept
{
  return _impl::runtimeEnvironment;
}

void Runtime::notImplemented(const char* who)
{
  Profiler::pushInstant(who, "NOT_IMPLEMENTED");
  _throw NotImplemented(who);
}

void Runtime::notSupported(const char* who)
{
  Profiler::pushInstant(who, "NOT_SUPPORTED");
  _throw NotSupported(who);
}

void Runtime::corruption(const char* who)
{
  Profiler::pushInstant(who, "CORRUPTION");
  _throw MemoryCorruption(who);
}

void Runtime::internalError(const char* who, const char* message)
{
  Profiler::pushInstant(who, "INTERNAL_ERROR");
  // TAG: add InternalError exception
  _throw UnexpectedFailure(message ? message : who);
}

class InitializeXeusCling {
public:
  
  InitializeXeusCling() noexcept
  {
    if (isRunningXeusCling()) {
      Runtime::setRuntimeEnvironment("xeus/cling");
    }
  }
};

InitializeXeusCling initializeXeusCling;

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
