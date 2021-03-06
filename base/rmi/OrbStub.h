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
#include <base/mem/Reference.h>
#include <base/rmi/OrbException.h>
#include <base/rmi/OrbChannel.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker stub.
  
  @short Object Request Broker stub.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbStub : public Object, public virtual ReferenceCountedObject {
protected:
  
  /** The ORB channel. */
  Reference<OrbChannel> channel;
  /** The reference of the object in the local address space. */
  OrbReference reference;
public:
  
  /**
    Initializes the ORB stub.
  */
  OrbStub() noexcept;
  
  /**
    Initializes the ORB stub.
  */
  inline OrbStub(Reference<OrbChannel> _channel, const OrbReference& _reference) noexcept
    : channel(_channel), reference(_reference) {
  }
  
  /**
    Returns the local reference to the object.
  */
  inline OrbReference getOrbReference() const noexcept {
    return reference;
  }
  
  /**
    Returns the fully qualified name of the object.
  */
  inline String getOrbIdentifier() const {
    return channel->getOrbIdentifier(*this);
  }
  
  /**
    Returns the fully qualified name of the interface.
  */
  inline String getOrbInterfaceIdentifier() const {
    return channel->getOrbInterfaceIdentifier(*this);
  }
  
  /**
    Destroys the ORB stub.
  */
  virtual ~OrbStub() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
