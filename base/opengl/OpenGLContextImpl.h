/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_OPENGL__OPEN_GL_CONTEXT_IMPL_H
#define _DK_SDU_MIP__BASE_OPENGL__OPEN_GL_CONTEXT_IMPL_H

#include <base/ui/Drawable.h>
#include <base/opengl/OpenGL.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Base class of all OpenGL rendering contexts.

  @short OpenGL rendering context.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OpenGLContextImpl : virtual public Drawable {
protected:

  void* graphicsContextHandle;
  /** Opaque handle to OpenGL rendering context. */
  void* renderingContextHandle;
  /** The number of overlay planes. */
  unsigned int numberOfOverlayPlanes;
  /** The number of underlay planes. */
  unsigned int numberOfUnderlayPlanes;
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
    Loads the OpenGL module.
  */
  static void loadModule() throw(OpenGLException);
public:

  /** OpenGL format flags. */
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
    SWAP_LAYER, /**< Swap individual layers. */
    DIRECT = UNDERLAY << 1, /**< Direct rendering support (accelerated). */
    GENERIC = DIRECT << 1 /**< Generic implementation. */
  };
  
  /**
    Initializes an OpenGL rendering context.
  */
  OpenGLContextImpl() throw(OpenGLException);

  /**
    Returns the vendor of the client.
  */
  String getGLClientVendor() const throw(OpenGLException);

  /**
    Returns the release of the client.
  */
  String getGLClientRelease() const throw(OpenGLException);

  /**
    Returns the client extensions.
  */
  String getGLClientExtensions() const throw(OpenGLException);

  /**
    Returns the vendor of the server.
  */
  String getGLServerVendor() const throw(OpenGLException);

  /**
    Returns the release of the server.
  */
  String getGLServerRelease() const throw(OpenGLException);

  /**
    Returns the server extensions.
  */
  String getGLServerExtensions() const throw(OpenGLException);
  
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
  
  inline bool hasAlphaBuffer() const throw() {
    return alphaBits;
  }
  
  inline bool hasAccumulatorBuffer() const throw() {
    return accumulatorBits;
  }
  
  inline bool hasDepthBuffer() const throw() {
    return depthBits;
  }
  
  inline bool hasStencilBuffer() const throw() {
    return stencilBits;
  }
  
  inline bool hasAuxBuffers() const throw() {
    return auxBuffers;
  }

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
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
