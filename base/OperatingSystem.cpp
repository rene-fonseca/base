/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OperatingSystem.h>
#include <base/Exception.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (defined(_DK_SDU_MIP__BASE__DEBUG) && (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32))

class CheckCondition {
public:

  CheckCondition(bool condition, const char* message) throw() {
    assert(condition, Exception(message));
  }
};

#if defined(_DK_SDU_MIP__BASE__DEBUG)
namespace { // anonymous namespace

  CheckCondition check(sizeof(OperatingSystem::Handle) == sizeof(HANDLE), "Type mismatch");
};
#endif // debug

#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
