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
#include <base/mem/ReferenceCountedObject.h>
#include <base/rmi/OrbException.h>
#include <base/rmi/OrbDecoder.h>
#include <base/rmi/OrbEncoder.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class OrbStub;

/**
  A bidirectional channel of communication.
  
  @short ORB channel.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbChannel : public Object, public ReferenceCountedObject {
private:

  /** Encoder. */
  Reference<OrbEncoder> encoder;
  /** Decoder. */
  Reference<OrbDecoder> decoder;
public:
  
  /**
    Initializes the channel.
  */
  OrbChannel(Reference<OrbEncoder> encoder, Reference<OrbDecoder> decoder) noexcept;
  
  /**
    Returns the fully qualified name of the specified stub.
  */
  String getOrbIdentifier(const OrbStub& object) const throw();

  /**
    Returns the fully qualified name of the interface of the specified stub.
  */
  String getOrbInterfaceIdentifier(const OrbStub& object) const throw();
  
  /**
    Destroys the channel.
  */
  virtual ~OrbChannel() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
