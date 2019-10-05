/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbResponse.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OrbResponse::OrbResponse() throw() {
}

OrbResponse::~OrbResponse() throw() {
}

/*
  what should be written to the channel
  channel->write(static_cast<unsigned int>(responseType));
  write value of response if primitive

  if exception
    stream exception
*/

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
