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

Exception::Exception() noexcept
  : cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
}

Exception::Exception(const char* _message) noexcept
  : message(_message),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
}

Exception::Exception(Type _type) noexcept
  : type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM)
{
}

Exception::Exception(const char* _message, Type _type) noexcept
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
      Exception e;
      e.setType(Type::getType<MyExceptionContext>());
      e.setCause(1);
      throw e;
      TEST_NOT_HERE(B);
    } catch (Exception& e) {
      TEST_ASSERT(e.getMessage() == nullptr);
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
