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
#include <base/Exception.h>
#include <base/UnitTest.h>

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

Exception::ExceptionHandler Exception::exceptionHandler = &defaultExceptionHandler;

Exception::Exception() noexcept
  : cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
#if 0 // || defined(_COM_AZURE_DEV__BASE__ANY_DEBUG) // TAG: also add support for Release config
  if (exceptionHandler) {
    exceptionHandler(this);
  }
#endif
}

Exception::Exception(const char* _message) noexcept
  : message(_message),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
#if 0 // || defined(_COM_AZURE_DEV__BASE__ANY_DEBUG) // TAG: also add support for Release config
  if (exceptionHandler) {
    exceptionHandler(this);
    // TAG: UnitTest should hook handler
  }
#endif
}

Exception::Exception(const Type& _type) noexcept
  : type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
#if 0 // && defined(_COM_AZURE_DEV__BASE__ANY_DEBUG) // TAG: also add support for Release config
  if (exceptionHandler) {
    exceptionHandler(this);
  }
#endif
}

Exception::Exception(const char* _message, const Type& _type) noexcept
  : message(_message),
    type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
}

Exception::Exception(const Exception& copy) noexcept
  : message(copy.message),
    type(copy.type),
    cause(copy.cause),
    error(copy.error)
{
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
