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
  /** Version of the OpenGL window management (e.g. 0x010300). */
  unsigned int version;
  /** Opaque handle to OpenGL rendering context. */
  void* renderingContext;
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
  OpenGLContext(const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException, UserInterfaceException);

  /**
    Returns the vendor of the client.
  */
  String getGLClientVendor() const throw(UserInterfaceException);

  /**
    Returns the release of the client.
  */
  String getGLClientRelease() const throw(UserInterfaceException);

  /**
    Returns the client extensions.
  */
  String getGLClientExtensions() const throw(UserInterfaceException);

  /**
    Returns the vendor of the server.
  */
  String getGLServerVendor() const throw(UserInterfaceException);

  /**
    Returns the release of the server.
  */
  String getGLServerRelease() const throw(UserInterfaceException);

  /**
    Returns the server extensions.
  */
  String getGLServerExtensions() const throw(UserInterfaceException);
  
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
    Returns true if context is a direct rendering context.
  */
  bool isDirect() const throw(OpenGLException);
  
  /**
    Returns true if the executing thread has an associated OpenGL context.
  */
  bool hasCurrent() const throw(OpenGLException);
  
  /**
    Returns true if the OpenGL context is the current context of the executing thread.
  */
  bool isCurrent() const throw(OpenGLException);
  
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
