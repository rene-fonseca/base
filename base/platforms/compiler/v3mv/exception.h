/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_V3MV__EXCEPTION_H
#define _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_V3MV__EXCEPTION_H

#include <base/platforms/features.h>
#include <base/Primitives.h>
#include <exception>
#include <typeinfo>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// this file is based on the information available at http://www.codesourcery.com/cxx-abi/abi-eh.html

namespace abi {
  
  enum _Unwind_Reason_Code {
    _URC_NO_REASON = 0,
    _URC_FOREIGN_EXCEPTION_CAUGHT = 1,
    _URC_FATAL_PHASE2_ERROR = 2,
    _URC_FATAL_PHASE1_ERROR = 3,
    _URC_NORMAL_STOP = 4,
    _URC_END_OF_STACK = 5,
    _URC_HANDLER_FOUND = 6,
    _URC_INSTALL_CONTEXT = 7,
    _URC_CONTINUE_UNWIND = 8
  };
  
  typedef void (*_Unwind_Exception_Cleanup_Fn)(_Unwind_Reason_Code reason, struct _Unwind_Exception* exc);
  
  struct _Unwind_Exception {
    uint64 exception_class;
	  _Unwind_Exception_Cleanup_Fn exception_cleanup;
	  uint64 private_1;
	  uint64 private_2;
  };
  
  struct __cxa_exception {
    const std::type_info* exceptionType;
    void (*exceptionDestructor)(void*);
    std::unexpected_handler unexpectedHandler;
    std::terminate_handler terminateHandler;
    const __cxa_exception* nextException;
    int handlerCount;
    int handlerSwitchValue;
    const char* actionRecord;
    const char* languageSpecificData;
    const void* catchTemp;
    const void* adjustedPtr;
    _Unwind_Exception unwindHeader;
  };
  
  struct __cxa_eh_globals {
    const __cxa_exception* caughtExceptions;
    unsigned int uncaughtExceptions;
  };
  
  extern "C" const __cxa_eh_globals* __cxa_get_globals();
  extern "C" const __cxa_eh_globals* __cxa_get_globals_fast();
  
}; // end of namespace abi

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#else
#  error Multiple inclusion of base/platforms/compiler/v3mv/exception.h
#endif
