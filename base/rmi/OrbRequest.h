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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker (ORB) request.
  
  @short Object Request Broker (ORB) request.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbRequest : public Object {
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
  int methodId = 0;
public:
  
  /**
    Initializes the request.
    
    @param stub The stub.
    @param method The name of the name.
  */
  OrbRequest(
    const Reference<OrbStub>& stub, const String& method);
  
  /**
    Writes invocation request.
  */
  inline void invoke() {
//     stub->invoke(methodId);
//     channel->writeRequest(INVOCATION);
//     channel->writeReference(reference);
//     channel->writeUInt16(methodId);
  }

  ~OrbRequest() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
