/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__COMPARABLE_H
#define _DK_SDU_MIP__BASE__COMPARABLE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Comparable interface.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Comparable {
public:

  /**
    Equality operator.

    @param object Object to be compared.
    @return True if the object are equal.
  */
  virtual bool operator==(const Comparable& comparable) const {return (this == &comparable);}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
