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

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The base class of polymorphic objects. Objects are not copyable by default.
  
  @short Polymorphic object.
  @version 1.0
*/

class Polymorphic {
private:

  /* Disable the default copy constructor. */
  Polymorphic(const Polymorphic& copy) throw();
  /* Disable the default assignment operator. */
  Polymorphic& operator=(const Polymorphic& eq) throw();
public:

  /**
    Initializes object.
  */
  inline Polymorphic() throw() {
  }
  
  /**
    Destroys the object.
  */
  virtual ~Polymorphic() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
