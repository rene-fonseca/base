/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/mathematics/Complex.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex& value) {
  return stream << "(" << value.getReal() << ";" << value.getImaginary() << ")";
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
