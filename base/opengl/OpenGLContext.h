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

#include <base/ui/WindowImpl.h>
#include <base/opengl/OpenGLContextImpl.h>
#include <base/collection/Array.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  OpenGL rendering context.

  @short OpenGL rendering context.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OpenGLContext : public WindowImpl, public OpenGLContextImpl {
private:

  nothing prefixInitialization;
  
  /**
    Initializes the OpenGL context.
  */
  nothing initialize(const Format& format) throw(OpenGLException, UserInterfaceException);
  
  /**
    Releases the rendering context.
  */
  void destroy() throw();
public:
  
  /** OpenGL property. */
  OpenGL openGL;
  
  /**
    Returns the available formats.
  */
  static Array<Format> getFormats(unsigned int flags) throw(OpenGLException, UserInterfaceException);
  
  /**
    Initializes a new OpenGL context.
  */
  OpenGLContext(const Position& position, const Dimension& dimension, const Format& format) throw(OpenGLException, UserInterfaceException);
  
  /**
    Invoked when the display should be updated.
  */
  void onDisplay() throw();
  
  /**
    Invoked when the window has been resized.

    @param dimension The new dimension of the window.
  */
  void onResize(const Dimension& dimension) throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
