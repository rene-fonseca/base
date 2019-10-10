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

#include <base/AutomationObject.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A property is an attribute which may be exposed using any appropriate access.
  Properties are required to prevent and/or handle wrong use properly.
  
  @code
  class MyProperty : public Property {
  public:
  };
  
  class MyClass : public Object {
  public:
  
    MyProperty property; // public accesss allowed
  };
  @endcode
  
  @short Property.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Property : public AutomationObject {
public:
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
