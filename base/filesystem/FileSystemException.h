/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_FILESYSTEM__FILE_SYSTEM_EXCEPTION_H
#define _DK_SDU_MIP__BASE_FILESYSTEM__FILE_SYSTEM_EXCEPTION_H

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Exception thrown on file object exceptions.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class FileSystemException : public Exception {
public:

  /**
    Initializes the exception object with no message.
  */
  FileSystemException() throw() : Exception() {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  FileSystemException(const char* message) throw() : Exception(message) {}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
