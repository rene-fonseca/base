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

#include <base/opengl/OpenGLContextImpl.h>
#include <base/ui/WindowImpl.h>

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
  nothing initialize() throw(UserInterfaceException);
  
  /**
    Releases the rendering context.
  */
  void destroy() throw();
public:

  /** OpenGL implementation. */
  OpenGL openGL;

  /** OpenGL support flags. */
  enum Flag {
    COLOR_INDEX = 1, /**< Color index. */
    DOUBLE_BUFFERED = COLOR_INDEX << 1, /**< Request double buffered. */
    ACCUMULATOR = DOUBLE_BUFFERED << 1, /**< Accumulator. */
    ALPHA = ACCUMULATOR << 1, /**< Request alpha/transparency support. */
    DEPTH = ALPHA << 1, /**< Depth buffer support. */
    STENCIL = DEPTH << 1, /**< Stencil buffer support. */
    AUX = STENCIL << 1, /**< Auxiliary buffer support. */
    MULTI_SAMPLE = AUX << 1, /**< Multi sample support. */
    STEREO = MULTI_SAMPLE << 1, /**< Stereo support. */
    LUMINANCE = STEREO << 1, /**< Luminance support. */
    RGB = LUMINANCE << 1,
    RGB15 = RGB << 1,
    RGB16 = RGB15 << 1,
    RGB24 = RGB16 << 1,
    RGBA32 = RGB24 << 1,
    OVERLAY = RGBA32 << 1,
    UNDERLAY = OVERLAY << 1,
    DIRECT = UNDERLAY << 1 /**< Direct rendering support. */
  };
  
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
