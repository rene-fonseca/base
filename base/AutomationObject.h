/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The base class of pure automation objects. Automation objects are neither
  copyable or dynamically construct able by default. Derived classes should not
  change these constraints.
  
  @short Pure automation object.
  @version 1.0
*/

class AutomationObject {
private:

  /** Prevent dynamic construction. */
  static void* operator new(MemorySize) throw() /*final*/;
  /** Prevent dynamic destruction. */
  static void operator delete(void*, MemorySize) throw() /*final*/;
  /** Prevent dynamic array construction. */
  static void* operator new[](MemorySize) throw() /*final*/;
  /** Prevent dynamic array destruction. */
  static void operator delete[](void*, MemorySize) throw() /*final*/;
  /** Prevent default copy construction. */
  AutomationObject(const AutomationObject& copy) throw() /*final*/;
  /** Prevent default assignment. */
  AutomationObject& operator=(const AutomationObject& eq) throw() /*final*/;
protected:
  
  /** Default initialization. */
  inline AutomationObject() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
