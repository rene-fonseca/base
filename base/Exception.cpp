/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Exception.h>

#if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV)
  #include <base/platforms/compiler/v3mv/exception.h> // includes private features
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

bool Exception::isUnwinding() throw() {
#if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV)
#  if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV_TRANSPARENT)
    const abi::__cxa_eh_globals* abi::__cxa_get_globals();
#  else
    // TAG: exception handling does not have to be "fast"
    const abi::__cxa_eh_globals* globals = abi::__cxa_get_globals_fast(); // __cxa_get_globals is invoked in Thread.cpp
#  endif
  const abi::__cxa_exception* caughtException = globals->caughtExceptions;
  return caughtException != 0;
#else
  return false;
#endif
}

Type Exception::getExceptionType() throw() {
#if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV)
#  if defined(_DK_SDU_MIP__BASE__EXCEPTION_V3MV_TRANSPARENT)
    const abi::__cxa_eh_globals* abi::__cxa_get_globals();
#  else
    // TAG: exception handling does not have to be "fast"
    const abi::__cxa_eh_globals* globals = abi::__cxa_get_globals_fast(); // __cxa_get_globals is called in Thread.cpp
#  endif
  const abi::__cxa_exception* caughtException = globals->caughtExceptions;
  if (caughtException) {
    return Type(caughtException->exceptionType);
  } else {
    return Type();
  }
#else
  return Type();
#endif
}

Exception::Exception() throw()
  : message(0),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM),
    error(0) {
}

Exception::Exception(const char* _message) throw()
  : message(_message),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM),
    error(0) {
}

Exception::Exception(Type _type) throw()
  : message(0),
    type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM),
    error(0) {
}

Exception::Exception(const char* _message, Type _type) throw()
  : message(_message),
    type(_type),
    cause(PrimitiveTraits<unsigned int>::MAXIMUM),
    error(0) {
}

Exception::Exception(const Exception& copy) throw()
  : message(copy.message),
    type(copy.type),
    cause(copy.cause),
    error(copy.error) {
}

Exception::~Exception() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
