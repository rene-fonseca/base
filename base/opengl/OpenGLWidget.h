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

#include <base/ui/Widget.h>
#include <base/opengl/OpenGLContextImpl.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  OpenGL widget.
  
  @short OpenGL widget.
  @ingroup ui opengl
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OpenGLWidget : public Widget, public OpenGLContextImpl {
private:

  nothing prefixInitialization;
  
  /**
    Initializes the OpenGL context.
  */
  nothing initialize(const Format& format);
  
  /**
    Releases the rendering context.
  */
  void destroy() noexcept;
public:

  /** OpenGL property. */
  OpenGL openGL;
  
  /**
    Initializes the OpenGL widget.
  */
  OpenGLWidget(Window& owner, const Format& format);

  /**
    Invoked when the widget should be repainted.
  */
  void onDisplay() noexcept;
  
  /**
    Destroys the widget.
  */
  inline ~OpenGLWidget() noexcept
  {
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
