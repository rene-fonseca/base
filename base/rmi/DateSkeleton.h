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

#include <base/rmi/OrbSkeleton.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker date skeleton.
  
  @short Object Request Broker date skeleton.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API DateSkeleton : public OrbSkeleton {
public:
  
  /**
    Initializes the date skeleton.
  */
  DateSkeleton() throw(OrbException);
  
  /**
    Returns the date.
  */
  long long getDate() const throw(OrbException);
  
  /**
    Destroys the date skeleton.
  */
  ~DateSkeleton() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
