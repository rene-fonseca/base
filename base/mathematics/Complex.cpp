/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/mathematics/Complex.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex& value) {
  return stream << "(" << value.getReal() << ";" << value.getImaginary() << ")";
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
