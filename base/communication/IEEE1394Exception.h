/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_EXCEPTION_H
#define _DK_SDU_MIP__BASE_COMMUNICATION__IEEE_1394_EXCEPTION_H

#include <base/communication/CommunicationsException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception raised by the IEEE 1394 classes.

  @short Exception raised by the IEEE 1394 classes.
  @ingroup exceptions communications
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IEEE1394Exception : public CommunicationsException {
public:

  /**
    Initializes the exception object with no message.
  */
  IEEE1394Exception() throw();

  /**
    Initializes exception object as unspecified.

    @param message The message.
  */
  IEEE1394Exception(const char* message) throw();

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  IEEE1394Exception(Type type) throw() : CommunicationsException(type) {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  IEEE1394Exception(const char* message, Type type) throw() : CommunicationsException(message, type) {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
