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
  unsigned int flags;
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
  /** The number of overlay planes. */
  unsigned int numberOfOverlayPlanes;
  /** The number of underlay planes. */
  unsigned int numberOfUnderlayPlanes;

  /**
    Loads the OpenGL module.
  */
  static void loadModule() throw(OpenGLException);
public:

  /** OpenGL format flags. */
  enum Flag {
    COLOR_INDEXED = 1, /**< Color index. */
    DOUBLE_BUFFERED = COLOR_INDEXED << 1, /**< Request double buffered. */
    ACCUMULATOR = DOUBLE_BUFFERED << 1, /**< Accumulator. */
    ALPHA = ACCUMULATOR << 1, /**< Request alpha/transparency support. */
    DEPTH = ALPHA << 1, /**< Depth buffer support. */
    STENCIL = DEPTH << 1, /**< Stencil buffer support. */
    AUX = STENCIL << 1, /**< Auxiliary buffer support. */
    MULTI_SAMPLE = AUX << 1, /**< Multi sample support. */
    STEREO = MULTI_SAMPLE << 1, /**< Stereo support. */
    LUMINANCE = STEREO << 1, /**< Luminance support. */
    RGB = LUMINANCE << 1,
    OVERLAY = RGB << 1,
    UNDERLAY = OVERLAY << 1,
    SWAP_LAYER, /**< Swap individual layers. */
    DIRECT = UNDERLAY << 1, /**< Direct rendering support (accelerated). */
    GENERIC = DIRECT << 1 /**< Generic implementation. */
  };

  /* Format descriptor. */
  struct Format {
    unsigned int id;
    unsigned int flags;
    uint8 colorBits;
    uint8 redBits;
    uint8 greenBits;
    uint8 blueBits;
    uint8 alphaBits;
    uint8 accumulatorBits;
    uint8 accumulatorRedBits;
    uint8 accumulatorGreenBits;
    uint8 accumulatorBlueBits;
    uint8 accumulatorAlphaBits;
    uint8 depthBits;
    uint8 stencilBits;
    uint8 auxBuffers;
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

  /**
    Returns true is the context is double buffered.
  */
  inline bool isDoubleBuffered() const throw() {
    return flags & DOUBLE_BUFFERED;
  }

  /**
    Returns true if the context is color indexed.
  */
  inline bool isColorIndexed() const throw() {
    return flags & COLOR_INDEXED;
  }

  /**
    Returns true if the context is stereoscopic.
  */
  inline bool isStereoscopic() const throw() {
    return flags & STEREO;
  }
  
  /**
    Returns true if the context is direct (accelerated).
  */
  inline bool isDirect() const throw() {
    return flags & DIRECT;
  }
  
  /**
    Returns true if the context is the generic implementation.
  */
  inline bool isGeneric() const throw() {
    return flags & GENERIC;
  }
  
  /**
    Returns true if the multi sample.
  */
  inline bool isMultiSampled() const throw() {
    return flags & MULTI_SAMPLE;
  }
  
  /**
    Returns true if the layers may be swapped individually.
  */
  inline bool isSwapLayers() const throw() {
    return flags & SWAP_LAYER;
  }
  
  /**
    Returns true is alpha buffer is available.
  */
  inline bool hasAlphaBuffer() const throw() {
    return alphaBits;
  }  

  /**
    Returns the number of color bits (excluding the alpha bits).
  */
  inline unsigned int getNumberOfColorBits() const throw() {
    return colorBits;
  }

  /**
    Returns the number of red bits.
  */
  inline unsigned int getNumberOfRedBits() const throw() {
    return redBits;
  }
  
  /**
    Returns the number of green bits.
  */
  inline unsigned int getNumberOfGreenBits() const throw() {
    return greenBits;
  }
  
  /**
    Returns the number of blue bits.
  */
  inline unsigned int getNumberOfBlueBits() const throw() {
    return blueBits;
  }

  /**
    Returns the number of alpha bits.
  */
  inline unsigned int getNumberOfAlphaBits() const throw() {
    return alphaBits;
  }

  /**
    Returns true if an accumulator buffer is available.
  */
  inline bool hasAccumulatorBuffer() const throw() {
    return accumulatorBits;
  }

  /**
    Returns the number of accumulator bits.
  */
  inline unsigned int getNumberOfAcculumatorBits() const throw() {
    return accumulatorBits;
  }

  /**
    Returns the number of red bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorRedBits() const throw() {
    return accumulatorRedBits;
  }
  
  /**
    Returns the number of green bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorGreenBits() const throw() {
    return accumulatorGreenBits;
  }
  
  /**
    Returns the number of blue bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorBlueBits() const throw() {
    return accumulatorBlueBits;
  }

  /**
    Returns the number of alpha bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorAlphaBits() const throw() {
    return accumulatorAlphaBits;
  }
  
  /**
    Returns true if depth buffer is available.
  */
  inline bool hasDepthBuffer() const throw() {
    return depthBits;
  }

  /**
    Returns the number of depth bits.
  */
  inline unsigned int getNumberOfDepthBits() const throw() {
    return depthBits;
  }

  /**
    Returns true if stencil buffer is available.
  */
  inline bool hasStencilBuffer() const throw() {
    return stencilBits;
  }

  /**
    Returns the number of stencil bits.
  */
  inline bool getNumberOfStencilBits() const throw() {
    return stencilBits;
  }

  /**
    Returns true if auxiliary buffers are available.
  */
  inline bool hasAuxBuffers() const throw() {
    return auxBuffers;
  }
  
  /**
    Returns the number of auxiliary buffers.
  */
  inline unsigned int getNumberOfAuxBuffers() const throw() {
    return auxBuffers;
  }
  
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
