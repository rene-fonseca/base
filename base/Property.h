/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__PROPERTY_H
#define _DK_SDU_MIP__BASE__PROPERTY_H

#include <base/AutomationObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Property : public AutomationObject {
public:
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
