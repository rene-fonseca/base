/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__RMI__ORB_ENCODING_H
#define _DK_SDU_MIP__BASE__RMI__ORB_ENCODING_H

#include <base/rmi/OrbEncoding.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String OrbEncoding::getUrnNamespace() throw() {
  return MESSAGE("urn:rmi:orb:encoding-scheme:");
}

OrbEncoding::~OrbEncoding() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
