/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/DynamicObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

bool DynamicObject::isValidObject() const throw() {
#if defined(DEBUG)
  const void* _this = this;
  return (_this != nullptr) && valid;
#else
  return true;
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
