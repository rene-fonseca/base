/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mathematics/IncompatibleOperands.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IncompatibleOperands::IncompatibleOperands() {
}

IncompatibleOperands::IncompatibleOperands(const char* message) : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
