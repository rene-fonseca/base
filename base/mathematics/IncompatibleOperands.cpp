/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mathematics/IncompatibleOperands.h>

IncompatibleOperands::IncompatibleOperands() {
}

IncompatibleOperands::IncompatibleOperands(const char* message) : Exception(message) {
}
