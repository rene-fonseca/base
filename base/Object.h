/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OBJECT_H
#define _DK_SDU_MIP__BASE__OBJECT_H

#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The base class of all non-abstract classes. Objects are not copyable by
  default.
  
  @short Object.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Object {
private:

  /* Disable the default copy constructor. */
  Object(const Object& copy) throw();
  /* Disable the default assignment operator. */
  Object& operator=(const Object& eq) throw();
public:

  /**
    Initializes object.
  */
  inline Object() throw() {
  }
};

template<>
class Uninitializeable<Object> {
public:

  static const bool IS_UNINITIALIZEABLE = Uninitializeable<bool>::IS_UNINITIALIZEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
