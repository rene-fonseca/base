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

/**
  This class prevents copy construction and default assignment.
  
  @short Not copyable.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API NotCopyable {
private:

  /**
    Default copy construction is not permitted.
  */
  NotCopyable(const NotCopyable& copy) throw();
  
  /**
    Default assignment is not permitted.
  */
  NotCopyable& operator=(const NotCopyable& eq) throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
