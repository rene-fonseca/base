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
#include <base/rmi/OrbDecoder.h>
#include <base/rmi/OrbEncoder.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker encoding scheme.
  
  @short Object Request Broker encoding scheme.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbEncoding : public Object, public virtual ReferenceCountedObject {
public:
  
  /**
    Returns the namespace of the URN identifying the encoding scheme. By
    convention all encoding schemes must be placed in the namespace
    "urn:rmi:orb:encoding-scheme:".
  */
  static String getUrnNamespace() noexcept;
  
  /**
    Returns a new decoder.
  */
  virtual Reference<OrbDecoder> getDecoder() const = 0;
  
  /**
    Returns a new encoder.
  */
  virtual Reference<OrbEncoder> getEncoder() const = 0;
  
  /**
    Returns the global identifier of the encoding scheme (e.g.
    "urn:rmi:orb:encoding-scheme:username@somedomain:Lisa").
  */
  virtual String getUrn() const noexcept = 0;
  
  /**
    Destroys the encoding.
  */
  virtual ~OrbEncoding() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
