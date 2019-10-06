/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class OrbStub;

/**
  A bidirectional channel of communication.
  
  @short ORB channel.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbChannel : public Object, public ReferenceCountedObject {
private:

  /** Encoder. */
  Reference<OrbEncoder> encoder;
  /** Decoder. */
  Reference<OrbDecoder> decoder;
public:
  
  /**
    Initializes the channel.
  */
  OrbChannel(Reference<OrbEncoder> encoder, Reference<OrbDecoder> decoder) throw();  
  
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
  virtual ~OrbChannel() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
