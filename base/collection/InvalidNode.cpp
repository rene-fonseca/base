/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/InvalidNode.h>

InvalidNode::InvalidNode() {
}

InvalidNode::InvalidNode(const char* message) : Exception(message) {
}
