/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_IO__END_OF_FILE_H
#define _DK_SDU_MIP__BASE_IO__END_OF_FILE_H

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Specifies that the end of stream has been reached unexpectedly during input.

  @author René Møller Fonseca
  @version 1.0
*/

class EndOfFile : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  EndOfFile();

  /**
    Initializes the exception object.

    @param message The message.
  */
  EndOfFile(const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
