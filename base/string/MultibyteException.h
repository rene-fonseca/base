/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_STRING__MULTIBYTE_EXCEPTION_H
#define _DK_SDU_MIP__BASE_STRING__MULTIBYTE_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This exception is thrown on convertion error from or to multibyte characters.

  @short Multibyte character conversion exception
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class MultibyteException : public Exception {
public:

};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
