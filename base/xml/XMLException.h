/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_EXCEPTION_H
#define _DK_SDU_MIP__BASE_XML__XML_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown by the XML classes.

  @author René Møller Fonseca
  @version 1.0
*/

class XMLException : public Exception {
public:

  inline XMLException() {}

  inline XMLException(const char* message) : Exception(message) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
