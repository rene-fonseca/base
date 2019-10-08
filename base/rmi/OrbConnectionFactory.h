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

#include <base/rmi/OrbConnection.h>
#include <base/mem/ProtectedPointer.h>
#include <base/mem/ReferenceCountedObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Orb;

/**
  ORB connection factory responsible for producing ORB connections initiated by
  an external source.
  
  @short ORB connection factory.
  @ingroup rmi
  @version 1.0
*/

class OrbConnectionFactory : public Object, public virtual ReferenceCountedObject {
public:
  
  /**
    Initializes the connection factory.
  */
  OrbConnectionFactory() throw();
  
  /**
    Returns the global identifier of the connection scheme (e.g.
    "urn:rmi:orb:scheme:tcpip").
  */
  virtual String getUrn() const throw() = 0;
  
  /**
    Returns the identifier of the end point.
  */
  virtual String getEndPoint() const throw() = 0;
  
  /**
    Invoked by the orb.
  */
  virtual void run(ProtectedPointer<Orb> orb) throw(OrbException) = 0;
  
  /**
    Destroys the connection factory.
  */
  virtual ~OrbConnectionFactory() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
