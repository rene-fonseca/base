/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_OPEN_GL__OPEN_GL_CONTEXT_H
#define _DK_SDU_MIP__BASE_OPEN_GL__OPEN_GL_CONTEXT_H

#include <base/opengl/OpenGL.h>
#include <base/ui/WindowImpl.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// TAG: put in primitives
struct Empty {
};

/**
  OpenGL rendering context.

  @short OpenGL rendering context.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OpenGLContext : public WindowImpl {
private:

  Empty prefixInitialization;
  /** Opaque handle to OpenGL rendering context. */
  void* renderingContext;
  void* drawable;
  /** The number of overlay planes. */
  unsigned int numberOfOverlayPlanes;
  /** The number of underlay planes. */
  unsigned int numberOfUnderlayPlanes;

  /**
    Initializes the OpenGL context.
  */
  Empty initialize() throw(UserInterfaceException);
  
  /**
    Releases the rendering context.
  */
  void onDestruction() throw();
public:

  /** OpenGL implementation. */
  OpenGL openGL;
  
  enum Flag {
    COLOR_INDEX = 1 << 0,
    DOUBLE_BUFFERED = 1 << 1,
    ACCUMULATOR = 1 << 2,
    ALPHA = 1 << 3,
    DEPTH = 1 << 4,
    STENCIL = 1 << 5,
    MULTI_SAMPLE = 1 << 6,
    STEREO = 1 << 7,
    LUMINANCE = 1 << 8
  };
  
  uint8 colorBits;
  uint8 redBits;
  uint8 redShift;
  uint8 greenBits;
  uint8 greenShift;
  uint8 blueBits;
  uint8 blueShift;
  uint8 alphaBits;
  uint8 alphaShift;
  uint8 accumulatorBits;
  uint8 accumulatorRedBits;
  uint8 accumulatorGreenBits;
  uint8 accumulatorBlueBits;
  uint8 accumulatorAlphaBits;
  uint8 depthBits;
  uint8 stencilBits;
  uint8 auxBuffers;
  
  /**
    Initializes a new OpenGL context.
  */
  OpenGLContext(const String& title, const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException, UserInterfaceException);
  
  /**
    Returns the number of overlay planes.
  */
  inline unsigned int getNumberOfOverlayPlanes() const throw(OpenGLException) {
    return numberOfOverlayPlanes;
  }

  /**
    Returns the number of underlay planes.
  */
  inline unsigned int getNumberOfUnderlayPlanes() const throw(OpenGLException) {
    return numberOfUnderlayPlanes;
  }

  inline bool isDoubleBuffered() const throw() {
    return false; // TAG: fixme
  }

  inline bool isRGBA() const throw() {
    return false; // TAG: fixme
  }

  // inline bool isHasAccumulatorBuffer() const throw();
  // inline bool isHasDepthBuffer() const throw();
  // inline bool isHasStencilBuffer() const throw();
  
  /**
    Selects this OpenGL context as the current context of the executing thread.
  */
  void makeCurrent() throw(OpenGLException);

  /**
    Deselects the OpenGL context of the executing thread.
  */
  void deselect() throw();

  /**
    Swaps all the buffers.
  */
  void swap() throw(OpenGLException);

  /**
    Swaps the specified layer.

    @param plane The layer to swap.
  */
  void swap(int plane) throw(OutOfRange, OpenGLException);
  
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
