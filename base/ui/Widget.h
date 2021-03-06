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

#include <base/ui/Window.h>
#include <base/ui/GraphicsContext.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The base class of all widgets.
  
  @short Widget base class.
  @ingroup ui
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Widget : public GraphicsContext {
private:
  
  /**
    Releases the widget.
  */
  void destroy() noexcept;
public:

  /**
    Initializes the widget.

    @param owner The owner of the widget.
  */
  Widget(Window& owner);

  /**
    Destroys the widget.
  */
  ~Widget() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
