/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  An object that can be draw-ed upon.
  
  @short A drawable object.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Drawable {
protected:

  /** Opaque handle to the object. */
  void* drawableHandle = nullptr;
public:

  inline Drawable() noexcept {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
