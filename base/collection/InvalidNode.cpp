/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/InvalidNode.h>

InvalidNode::InvalidNode() {
}

InvalidNode::InvalidNode(const char* message) : Exception(message) {
}
