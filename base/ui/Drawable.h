/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  An object that can be draw-ed upon.
  
  @short A drawable object.
  @ingroup ui
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Drawable {
protected:

  /** Opaque handle to the object. */
  void* drawableHandle = nullptr;
public:

  inline Drawable() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
