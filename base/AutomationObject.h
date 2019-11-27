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

#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The base class of pure automation objects. Automation objects are neither
  copyable or dynamically construct able by default. Derived classes should not
  change these constraints.
  
  @short Pure automation object.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API AutomationObject {
private:

  /** Prevent dynamic construction. */
  static void* operator new(MemorySize) noexcept /*final*/;
  /** Prevent dynamic destruction. */
  static void operator delete(void*, MemorySize) noexcept /*final*/;
  /** Prevent dynamic array construction. */
  static void* operator new[](MemorySize) noexcept /*final*/;
  /** Prevent dynamic array destruction. */
  static void operator delete[](void*, MemorySize) noexcept /*final*/;
  /** Prevent default copy construction. */
  AutomationObject(const AutomationObject& copy) noexcept /*final*/;
  /** Prevent default assignment. */
  AutomationObject& operator=(const AutomationObject& assign) noexcept /*final*/;
protected:
  
  /** Default initialization. */
  inline AutomationObject() noexcept
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
