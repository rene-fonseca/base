/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_EXCEPTION_H
#define _DK_SDU_MIP__BASE_XML__XML_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown by the XML classes.

  @ingroup exceptions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class XMLException : public Exception {
public:

  inline XMLException() throw() {}

  inline XMLException(const char* message) throw() : Exception(message) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
