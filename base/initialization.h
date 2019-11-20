/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class CallOnDestruct {
private:

  typedef void (*Function)();

  Function function = nullptr;
public:

  CallOnDestruct(Function _function)
    : function(_function)
  {
    BASSERT(function);
  }

  ~CallOnDestruct()
  {
    (*function)();
  }
};

/** Internal class. */
class _COM_AZURE_DEV__BASE__API Cleanup {
public:

  typedef void (*CleanupFunction)();

  static bool registerCleanup(CleanupFunction function) noexcept;
};

/** Internal class. */
class _COM_AZURE_DEV__BASE__API RegisterCleanup {
public:

  inline RegisterCleanup(Cleanup::CleanupFunction function)
  {
    Cleanup::registerCleanup(function);
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
