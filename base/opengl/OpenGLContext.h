/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_OPENGL__OPEN_GL_CONTEXT_H
#define _DK_SDU_MIP__BASE_OPENGL__OPEN_GL_CONTEXT_H

#include <base/ui/WindowImpl.h>
#include <base/opengl/OpenGLContextImpl.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  OpenGL rendering context.

  @short OpenGL rendering context.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OpenGLContext : public WindowImpl, public OpenGLContextImpl {
private:

  nothing prefixInitialization;
  
  /**
    Initializes the OpenGL context.
  */
  nothing initialize() throw(OpenGLException, UserInterfaceException);
  
  /**
    Releases the rendering context.
  */
  void destroy() throw();
public:

  /** OpenGL implementation. */
  OpenGL openGL;
  
  /**
    Initializes a new OpenGL context.
  */
  OpenGLContext(const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException, UserInterfaceException);
  
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
