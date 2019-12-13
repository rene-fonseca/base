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

#include <base/rmi/OrbConnection.h>
#include <base/mem/ProtectedPointer.h>
#include <base/mem/ReferenceCountedObject.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Orb;

/**
  ORB connection factory responsible for producing ORB connections initiated by
  an external source.
  
  @short ORB connection factory.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbConnectionFactory : public Object, public virtual ReferenceCountedObject {
public:
  
  /**
    Initializes the connection factory.
  */
  OrbConnectionFactory() noexcept;
  
  /**
    Returns the global identifier of the connection scheme (e.g.
    "urn:rmi:orb:scheme:tcpip").
  */
  virtual String getUrn() const noexcept = 0;
  
  /**
    Returns the identifier of the end point.
  */
  virtual String getEndPoint() const noexcept = 0;
  
  /**
    Invoked by the orb.
  */
  virtual void run(ProtectedPointer<Orb> orb) throw(OrbException) = 0;
  
  /**
    Destroys the connection factory.
  */
  virtual ~OrbConnectionFactory() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
