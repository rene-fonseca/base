/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_EXCEPTION_H
#define _DK_SDU_MIP__BASE_IO__FILE_EXCEPTION_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on file object exceptions.

  @author René Møller Fonseca
  @version 1.0
*/

class FileException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  FileException() throw() : IOException() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  FileException(const char* message) throw() : IOException(message) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
