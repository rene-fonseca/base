/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Dimension.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FormatOutputStream& operator<<(FormatOutputStream& stream, const Dimension& value) {
  return stream << '(' << value.width << ',' << value.height << ')';
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
