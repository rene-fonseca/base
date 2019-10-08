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

#include <base/rmi/OrbStub.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker date stub.
  
  @short Object Request Broker date stub.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API OrbDateStub : public OrbStub {
public:
  
  /**
    Initializes the date stub.
  */
  OrbDateStub() throw(OrbException);
  
  /**
    Returns the date.
  */
  long long getDate() const throw(OrbException);
  
  /**
    Destroys the date stub.
  */
  ~OrbDateStub() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
