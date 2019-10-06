/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The base class of all dynamically allocated classes.

  @short Dynamic object.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class DynamicObject {
private:

#if defined(DEBUG)
  bool valid = true;
#endif
public:

  /**
    Initializes the object.
  */
  inline DynamicObject() throw() {
  }

  /**
    Returns true if the object is valid.
  */
  bool isValidObject() const throw();

  /**
    Destroys the object.
  */
  virtual inline ~DynamicObject() {
#if defined(DEBUG)
    ASSERT(valid);
    valid = false;
#endif
  }
};

template<>
class Uninitializeable<DynamicObject> {
public:

  static const bool IS_UNINITIALIZEABLE = Uninitializeable<bool>::IS_UNINITIALIZEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
