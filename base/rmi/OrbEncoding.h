/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_ENCODING_H
#define _DK_SDU_MIP__BASE_RMI__ORB_ENCODING_H

#include <base/Object.h>
#include <base/mem/Reference.h>
#include <base/rmi/OrbDecoder.h>
#include <base/rmi/OrbEncoder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker encoding scheme.
  
  @short Object Request Broker encoding scheme.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbEncoding : public Object, public virtual ReferenceCountedObject {
public:
  
  /**
    Returns the namespace of the URN identifying the encoding scheme. By
    convention all encoding schemes must be placed in the namespace
    "urn:rmi:orb:encoding-scheme:".
  */
  static String getUrnNamespace() throw();
  
  /**
    Returns a new decoder.
  */
  virtual Reference<OrbDecoder> getDecoder() const throw();
  
  /**
    Returns a new encoder.
  */
  virtual Reference<OrbEncoder> getEncoder() const throw();
  
  /**
    Returns the global identifier of the encoding scheme (e.g.
    "urn:rmi:orb:encoding-scheme:fonseca@mip.sdu.dk:Lisa").
  */
  virtual String getUrn() const throw() = 0;
  
  /**
    Destroys the encoding.
  */
  virtual ~OrbEncoding() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
