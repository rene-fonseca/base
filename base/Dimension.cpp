/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Dimension.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value) {
  return stream << '(' << value.width << ',' << value.height << ')';
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
