/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Construct.h>

Construct::Construct() throw() {
}

Construct::Construct(const char* message) throw() : Exception(message) {
}
