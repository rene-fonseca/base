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

#include <base/ui/GraphicsContext.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Window.
  
  @short Window.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Window : public GraphicsContext {
  friend class Widget;
private:
  
  /**
    Releases the rendering context.
  */
  void destroy() noexcept;
public:
  
  /**
    Initializes a window.

    @param flags The flags.
  */
  Window(unsigned int flags) throw(UserInterfaceException);
  
  /**
    Initializes a window.

    @param position The initial position.
    @param dimension The initial dimension.
    @param flags The flags.
  */
  Window(
    const Position& position,
    const Dimension& dimension,
    unsigned int flags) throw(UserInterfaceException);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
