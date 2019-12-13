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
  Pop up window.
  
  @short Pop up window.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API PopUpWindow : public GraphicsContext {
private:
  
  /**
    Releases the rendering context.
  */
  void destroy() noexcept;
public:
  
  /**
    Initializes a pop up window.

    @param flags The flags.
  */
  PopUpWindow(unsigned int flags);
  
  /**
    Initializes a pop up window.

    @param position The initial position.
    @param dimension The initial dimension.
    @param flags The flags.
  */
  PopUpWindow(const Position& position, const Dimension& dimension, unsigned int flags);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
