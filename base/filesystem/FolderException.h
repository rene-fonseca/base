/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FOLDER_EXCEPTION_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FOLDER_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on folder object exceptions.

  @author René Møller Fonseca
  @version 1.0
*/

class FolderException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  FolderException() throw() : Exception() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  FolderException(const char* message) throw() : Exception(message) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
