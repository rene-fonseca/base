/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/EndOfFile.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

EndOfFile::EndOfFile() {
}

EndOfFile::EndOfFile(const char* message) : IOException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
