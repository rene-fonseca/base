/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_REQUEST_H
#define _DK_SDU_MIP__BASE_RMI__ORB_REQUEST_H

#include <base/rmi/OrbStub.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker (ORB) request.
  
  @short Object Request Broker (ORB) request.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbRequest : public Object {
public:
  
  /** Request id. */
  enum RequestType {
    LOOKUP_OBJECT = 1, /**< Lookup object reference request. */
    LOOKUP_METHOD = 2, /**< Lookup method identifier request. */
    INVOCATION = 3 /**< Invocation request. */
  };
private:
  
  /** The stub. */
  Reference<OrbStub> stub;
  /** The id of the method. */
  int methodId;
public:
  
  /**
    Initializes the request.
    
    @param stub The stub.
    @param method The name of the name.
  */
  OrbRequest(
    const Reference<OrbStub>& stub, const String& method) throw(OrbException);
  
  /**
    Writes invocation request.
  */
  inline void invoke() throw(OrbException) {
//     stub->invoke(methodId);
//     channel->writeRequest(INVOCATION);
//     channel->writeReference(reference);
//     channel->writeUInt16(methodId);
  }

  ~OrbRequest() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
