/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__FILE_NOT_FOUND_H
#define _DK_SDU_MIP__BASE_IO__FILE_NOT_FOUND_H

#include <base/io/FileException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Specifies that the file doesn't exist or couldn't be opened.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileNotFound : public FileException {
public:

  /**
    Initializes the exception object with no message.
  */
  FileNotFound();

  /**
    Initializes the exception object.

    @param message The message.
  */
  FileNotFound(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
