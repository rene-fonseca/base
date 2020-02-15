/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/concurrency/ThreadLocalContext.h>
#include <base/Exception.h>
#include <base/UnitTest.h>
#include <base/string/ANSIEscapeSequence.h>
#include <base/StackFrame.h>
#include <memory>
#include <system_error>

#if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV)
  #include <base/platforms/compiler/v3mv/exception.h> // includes private features
#endif

#include <base/string/String.h>
#include <base/StackFrame.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

DumpStackOnException::~DumpStackOnException() noexcept
{
  if (Exception::hasPendingException()) {
    // throw UnwindException("Unexpected exception unwinding during destructor.");
    ferr << "Internal error: Unexpected exception unwinding." << ENDL;
    StackFrame::dump();
  }
}

// TAG: allocators should dump stack on destruction failure - how do we do it safely - need to destruct existing objects - but these can fail too
// TAG: throw AllocatorException("Failed to construct object."); - but this will hide causing exception

bool Exception::hasPendingException() noexcept
{
  std::exception_ptr exception = std::current_exception();
  return exception != nullptr;
}

bool Exception::isUnwinding() noexcept
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IOS)
  return false;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD)
  return std::uncaught_exception();
#else
  return std::uncaught_exceptions() > 0;
#endif
}

unsigned int Exception::getPendingExceptions() noexcept
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IOS)
  return 0;
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD)
  return std::uncaught_exception() ? 1 : 0;
#else
  int result = std::uncaught_exceptions();
  BASSERT(result >= 0);
  return result;
#endif
}

const char* Exception::getStdExceptionName(const std::exception& e) noexcept
{
  if (dynamic_cast<const std::runtime_error*>(&e)) {
    if (dynamic_cast<const std::range_error*>(&e)) {
      return "std::range_error";
    } else if (dynamic_cast<const std::overflow_error*>(&e)) {
      return "std::overflow_error";
    } else if (dynamic_cast<const std::underflow_error*>(&e)) {
      return "std::underflow_error";
    } else if (dynamic_cast<const std::out_of_range*>(&e)) {
      return "std::out_of_range";
#if 0
    } else if (dynamic_cast<const std::regex_error*>(&e)) { // C++11
      return "std::regex_error";
#endif
    } else if (dynamic_cast<const std::system_error*>(&e)) { // C++11
      return "std::system_error";
    }
    return "std::runtime_error";
  } else if (dynamic_cast<const std::logic_error*>(&e)) {
    if (dynamic_cast<const std::invalid_argument*>(&e)) {
      return "std::invalid_argument";
    } else if (dynamic_cast<const std::domain_error*>(&e)) {
      return "std::domain_error";
    } else if (dynamic_cast<const std::length_error*>(&e)) {
      return "std::length_error";
    } else if (dynamic_cast<const std::out_of_range*>(&e)) {
      return "std::out_of_range";
#if 0
    } else if (dynamic_cast<const std::future_error*>(&e)) { // C++11
      return "std::future_error";
#endif
    }
    return "std::logic_error";
  } else if (dynamic_cast<const std::bad_typeid*>(&e)) {
    return "std::bad_typeid";
  } else if (dynamic_cast<const std::bad_cast*>(&e)) {
    return "std::bad_cast";
  } else if (dynamic_cast<const std::bad_weak_ptr*>(&e)) { // C++11
    return "std::bad_weak_ptr";
#if 0
  } else if (dynamic_cast<const std::bad_function_call*>(&e)) { // C++11
    return "std::bad_function_call";
#endif
  } else if (dynamic_cast<const std::bad_alloc*>(&e)) {
    return "std::bad_alloc";
  } else if (dynamic_cast<const std::bad_array_new_length*>(&e)) { // C++11
    return "std::bad_array_new_length";
  } else if (dynamic_cast<const std::bad_exception*>(&e)) {
    return "std::bad_exception";
  }
  return "std::exception";
}

Type Exception::getExceptionType() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV)
#  if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV_TRANSPARENT)
    const abi::__cxa_eh_globals* abi::__cxa_get_globals();
#  else
    // TAG: exception handling does not have to be "fast"
    const abi::__cxa_eh_globals* globals = abi::__cxa_get_globals_fast(); // __cxa_get_globals is called in Thread.cpp
#  endif
  const abi::__cxa_exception* caughtException = globals->caughtExceptions;
  if (caughtException) {
    // ferr << caughtException->nextException << ENDL;
    return Type(caughtException->exceptionType);
  } else {
    return Type();
  }
#else
  return Type();
#endif
}

extern void defaultExceptionHandler(const Exception* exception) noexcept;

bool Exception::dumpExceptions = false;

#if 1 || defined(_COM_AZURE_DEV__BASE__ANY_DEBUG) // enabled for release builds also for now
Exception::ExceptionHandler Exception::exceptionHandler = &defaultExceptionHandler;
#else
Exception::ExceptionHandler Exception::exceptionHandler = nullptr;
#endif

Exception::StackTrace Exception::getStackTrace()
{
  // can we clear trace after exception is caught - but rethrow should keep it - so can only clear on final and no more unwinding
  StackTrace result;
  auto tls = Thread::getLocalContext();
  if (tls) {
    auto stackTrace = tls->stackTrace.getTrace();
    result.begin = stackTrace.begin();
    result.end = stackTrace.end();
  }
  return result;
}

// TAG: can we hook and detect throw of non-base exception: void __cxa_throw (void *thrown_exception, std::type_info *tinfo, void (*dest) (void *));

// C++: it would be preferred to have handler for throw like std::set_terminate() but not a full handler just a hook
// to allow strack trace tracking for easy debugging - the hook would also handle rethrows
// It would be nice if we could do a dynamic cast to the type of the thrown exception to be able to add some
// meta info (counter stats, stack trace, ...) to it before unwinding gets initiated
// throw_handler* std::set_throw_handler(throw_handler*) noexcept;
// throw_handler* std::get_throw_handler() noexcept;
void Exception::onThrow(const Exception& exception) noexcept
{
  if (Runtime::getRuntimeEnvironment()) {
    fout << "Throwing: " << exception << ENDL; // print until supported
    StackFrame::dump();
  }
  
  if (!SUPPORTS_EXCEPTIONS) { // we could allow this to be an option
    fout << "Throwing: " << exception << ENDL; // print until supported
    // StackFrame::dump();
  }
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(&exception);
  }
}

void Exception::rethrow()
{
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    // C++: provide throw_cast to avoid rethrow for casting to given type
    // see https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html#base-throw
    // std::type_info* currentType = __cxa_current_exception_type();
    // auto type = &typeid(Exception);
    // FYI dynamic_cast<void*>(object) returns most derived type
    // typeid(*src) - may return most derived type - need to confirm
    // TAG: alternatively just traverse all approved/registered exceptions
    // std::type_info* currentType = __cxa_current_exception_type();
    try {
      throw; // calls std::terminate() if no exception
    } catch (Exception& e) {
      exceptionHandler(&e);
      throw;
#if 0 && defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
    } catch (std::exception& e) {
      throw;
    } catch (...) {
      BASSERT(!"Invalid exception.");
#endif
    }
    // TAG: detect if std::exception or base::Exception
#if 0
    auto exception = std::current_exception(); // C++: would be nice to be able to dynamic cast to type
    if (INLINE_ASSERT(exception)) {
      std::rethrow_exception(exception);
    } else {
      // no exception
    }
#endif
  }
  
  throw;
}

Exception::Exception() noexcept
  : cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
#if 0
  _COM_AZURE_DEV__BASE__PRINT("Exception::Exception()\n");
  // we cannot use 'this'
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
#endif
}

Exception::Exception(const char* _message) noexcept
  : message(_message),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
#if 0
  _COM_AZURE_DEV__BASE__PRINT("Exception::Exception(%s)\n", _message);
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
#endif
}

Exception::Exception(const Type& _type) noexcept
  : type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
#if 0
  _COM_AZURE_DEV__BASE__PRINT("Exception::Exception(type=%s)\n", type.getLocalName());
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
#endif
}

Exception::Exception(const char* _message, const Type& _type) noexcept
  : message(_message),
    type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
#if 0
  _COM_AZURE_DEV__BASE__PRINT("Exception::Exception(%s, type=%s)\n", _message, type.getLocalName());
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
#endif
}

Exception::Exception(const Exception& copy) noexcept
  : message(copy.message),
    type(copy.type),
    cause(copy.cause),
    error(copy.error)
{
  // undesired case for exception handler
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
  copies = copy.copies + 1;

  static MemorySize total = 0;
  ++total;

  ferr << "WARNING: UNDESIRED COPY OF EXCEPTION: " << copies << "/" << total << ENDL;
  StackFrame::dump();
#endif
#if 0 && defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
  // ferr << "WARNING: UNDESIRED COPY OF EXCEPTION" << ENDL;
  // StackFrame::dump();
#endif
}

Exception::Exception(Exception&& move) noexcept
  : message(move.message),
    type(move.type),
    cause(move.cause),
    error(move.error)
{
  // undesired case for exception handler
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
  copies = move.copies;
  move.copies = 0;
#endif
}

Type Exception::getThisType() const noexcept
{
  return Type::getType(*this);
}

bool Exception::isCommonException() const noexcept
{
  return false;
}

Exception::~Exception() noexcept
{
}

void ThrowException::onException(const char* who, const char* file, unsigned int line) noexcept
{
  if (!Exception::getDumpExceptions()) {
    return;
  }

  if (auto tlc = Thread::getLocalContext()) {
    tlc->numberOfExceptions += 1;
  }
  
  file = Debug::getRelativePath(file);
#if 0 && (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  GlobalPrint::printf("THROW in %s at %s:%d\n", who, file, line);
#else
  auto& stream = fout;

  const bool colors = stream.isANSITerminal();
  String _who = who;
  stream << "THROW in ";
  if (colors) {
    auto index = _who.indexOf('(');
    if (index >= 0) {
      auto index2 = _who.indexOf(')', index + 1);
      if (index2 < 0) {
        index2 = _who.getLength();
      }
      stream << setForeground(ANSIEscapeSequence::BLUE) << bold() << _who.substring(0, index) << normal()
             << setForeground(ANSIEscapeSequence::BLUE) << italic() << _who.substring(index, index2 + 1)
             << setForeground(ANSIEscapeSequence::BLUE) << bold() << _who.substring(index2 + 1);
    } else {
      stream << setForeground(ANSIEscapeSequence::BLUE) << bold() << _who;
    }
    stream << normal();
  } else {
    stream << who;
  }
  stream << " at ";
  if (colors) {
    stream << setForeground(ANSIEscapeSequence::WHITE) << bold();
  }
  stream << file << ":" << line;
  if (colors) {
    stream << normal() << ENDL;
  }
  
  if (StackTrace::doesSupportStackTrace()) {
    static bool showStackTrace = true;
    if (showStackTrace) {
      stream << FormattedStackTrace(StackTrace::getStack(), StackTrace::FLAG_DEFAULT |
        (colors ? StackTrace::FLAG_USE_COLORS : 0));
    }
  }
#endif
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class MyExceptionContext {};

class MyException : public Exception {
public:
};

class TEST_CLASS(Exception) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base");

  void run() override
  {
    Exception e1;
    Exception e2("123");
    Exception e3("123", Type::getType<Exception>());

#if (!defined(_COM_AZURE_DEV__BASE__NO_EXCEPTIONS)) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__SPARC64) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__S390X)
    TEST_DECLARE_HERE(A);
    TEST_DECLARE_NOT_HERE(B);
    TEST_DECLARE_HERE(C);
    
    try {
      _throw bindException(bindType(MyException(), Type::getType<MyExceptionContext>()), "My message.", 1);
      TEST_NOT_HERE(B);
    } catch (MyException& e) {
      const char* text = "My message.";
      TEST_ASSERT(e.getMessage() == text); // compare pointers!
      TEST_ASSERT(e.getCause() == 1);
      TEST_ASSERT(e.getType() == Type::getType<MyExceptionContext>());
      TEST_HERE(A);
    }

    try {
      _throw Exception("Message.");
    } catch (const Exception& e) {
      TEST_HERE(C);
      TEST_ASSERT(String(e.getMessage()) == String("Message."));
    }
    
    // TAG: test constructor and destructor exception
#endif
  }
};

TEST_REGISTER(Exception);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
