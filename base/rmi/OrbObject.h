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

#include <base/Object.h>
#include <base/rmi/OrbException.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker object.
  
  @short Object Request Broker object.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbObject : public Object {
public:
  
  /**
    Initializes the orb object.
  */
  OrbObject();
  
  /**
    Returns the identifier of the object.
  */
  String getOrbIdentifier() const;
  
  /**
    Destroys the orb object.
  */
  virtual ~OrbObject() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
