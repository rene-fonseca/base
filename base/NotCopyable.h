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
  This class prevents copy construction and default assignment.
  
  @short Not copyable.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class NotCopyable {
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
