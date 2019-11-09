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
#include <base/concurrency/Thread.h>
#include <base/Exception.h>
#include <base/UnitTest.h>
#include <memory>

#if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV)
  #include <base/platforms/compiler/v3mv/exception.h> // includes private features
#endif

#include <base/string/String.h>
#include <base/StackFrame.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

bool Exception::isUnwinding() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV)
#  if defined(_COM_AZURE_DEV__BASE__EXCEPTION_V3MV_TRANSPARENT)
    const abi::__cxa_eh_globals* abi::__cxa_get_globals();
#  else
    // TAG: exception handling does not have to be "fast"
    const abi::__cxa_eh_globals* globals = abi::__cxa_get_globals_fast(); // __cxa_get_globals is invoked in Thread.cpp
#  endif
  const abi::__cxa_exception* caughtException = globals->caughtExceptions;
  if (caughtException) {
    // ferr << caughtException->nextException << ENDL;
  }
  return caughtException != 0;
#else
  return false;
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

extern void defaultExceptionHandler(Exception* exception) noexcept;

bool Exception::dumpExceptions = false;

#if 1 || defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
Exception::ExceptionHandler Exception::exceptionHandler = &defaultExceptionHandler;
#else
Exception::ExceptionHandler Exception::exceptionHandler = nullptr;
#endif

#if 0
template<class TYPE>
class Span {
public:

  const TYPE* begin = nullptr;
  const TYPE* end = nullptr;

  inline bool empty() const noexcept
  {
    return end == begin;
  }

  inline MemoryDiff size() const noexcept
  {
    return end - begin;
  }
};
#endif

Exception::StackTrace Exception::getStackTrace()
{
  // TAG: can we clear trace at after exception is caught
  StackTrace result;
  auto tls = Thread::getLocalContext();
  if (tls) {
    const auto& stackTrace = tls->stackTrace;
    result.begin = stackTrace.getTrace();
    result.end = result.begin + stackTrace.getSize();
  }
  return result;
}

Exception::Exception() noexcept
  : cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
}

Exception::Exception(const char* _message) noexcept
  : message(_message),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
}

Exception::Exception(const Type& _type) noexcept
  : type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
}

Exception::Exception(const char* _message, const Type& _type) noexcept
  : message(_message),
    type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
  if (exceptionHandler) { // not installed for release builds - but can be installed at runtime
    exceptionHandler(this);
  }
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
#endif
}

Type Exception::getThisType() const noexcept
{
  return Type::getType(*this);
}

Exception::~Exception() noexcept
{
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
    TEST_DECLARE_HERE(A);
    TEST_DECLARE_NOT_HERE(B);
    TEST_DECLARE_HERE(C);

    try {
      throw bindException(bindType(MyException(), Type::getType<MyExceptionContext>()), "My message.", 1);
      TEST_NOT_HERE(B);
    } catch (MyException& e) {
      TEST_ASSERT(e.getMessage() == "My message."); // compare pointers!
      TEST_ASSERT(e.getCause() == 1);
      TEST_ASSERT(e.getType() == Type::getType<MyExceptionContext>());
      TEST_HERE(A);
    }

    try {
      throw Exception("Message");
    } catch (const Exception& e) {
      TEST_HERE(C);
      TEST_ASSERT(String(e.getMessage()) == String("Message"));
    }
  }
};

TEST_REGISTER(Exception);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
