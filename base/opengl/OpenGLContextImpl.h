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

#include <base/ui/Drawable.h>
#include <base/opengl/OpenGL.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Base class of all OpenGL rendering contexts.

  @short OpenGL rendering context.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OpenGLContextImpl : virtual public Drawable {
protected:

  void* graphicsContextHandle = nullptr;
  /** Opaque handle to OpenGL rendering context. */
  void* renderingContextHandle = nullptr;
  unsigned int flags = 0;
  uint8 colorBits = 0;
  uint8 redBits = 0;
  uint8 redShift = 0;
  uint8 greenBits = 0;
  uint8 greenShift = 0;
  uint8 blueBits = 0;
  uint8 blueShift = 0;
  uint8 alphaBits = 0;
  uint8 alphaShift = 0;
  uint8 accumulatorBits = 0;
  uint8 accumulatorRedBits = 0;
  uint8 accumulatorGreenBits = 0;
  uint8 accumulatorBlueBits = 0;
  uint8 accumulatorAlphaBits = 0;
  uint8 depthBits = 0;
  uint8 stencilBits = 0;
  uint8 auxBuffers = 0;
  /** The number of overlay planes. */
  unsigned int numberOfOverlayPlanes = 0;
  /** The number of underlay planes. */
  unsigned int numberOfUnderlayPlanes = 0;

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
  class _COM_AZURE_DEV__BASE__API Format {
  public:

    unsigned int id = 0;
    unsigned int flags = 0;
    uint8 colorBits = 0;
    uint8 redBits = 0;
    uint8 greenBits = 0;
    uint8 blueBits = 0;
    uint8 alphaBits = 0;
    uint8 accumulatorBits = 0;
    uint8 accumulatorRedBits = 0;
    uint8 accumulatorGreenBits = 0;
    uint8 accumulatorBlueBits = 0;
    uint8 accumulatorAlphaBits = 0;
    uint8 depthBits = 0;
    uint8 stencilBits = 0;
    uint8 auxBuffers = 0;
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
  inline bool isDoubleBuffered() const noexcept {
    return flags & DOUBLE_BUFFERED;
  }

  /**
    Returns true if the context is color indexed.
  */
  inline bool isColorIndexed() const noexcept {
    return flags & COLOR_INDEXED;
  }

  /**
    Returns true if the context is stereoscopic.
  */
  inline bool isStereoscopic() const noexcept {
    return flags & STEREO;
  }
  
  /**
    Returns true if the context is direct (accelerated).
  */
  inline bool isDirect() const noexcept {
    return flags & DIRECT;
  }
  
  /**
    Returns true if the context is the generic implementation.
  */
  inline bool isGeneric() const noexcept {
    return flags & GENERIC;
  }
  
  /**
    Returns true if the multi sample.
  */
  inline bool isMultiSampled() const noexcept {
    return flags & MULTI_SAMPLE;
  }
  
  /**
    Returns true if the layers may be swapped individually.
  */
  inline bool isSwapLayers() const noexcept {
    return flags & SWAP_LAYER;
  }
  
  /**
    Returns true is alpha buffer is available.
  */
  inline bool hasAlphaBuffer() const noexcept {
    return alphaBits;
  }  

  /**
    Returns the number of color bits (excluding the alpha bits).
  */
  inline unsigned int getNumberOfColorBits() const noexcept {
    return colorBits;
  }

  /**
    Returns the number of red bits.
  */
  inline unsigned int getNumberOfRedBits() const noexcept {
    return redBits;
  }
  
  /**
    Returns the number of green bits.
  */
  inline unsigned int getNumberOfGreenBits() const noexcept {
    return greenBits;
  }
  
  /**
    Returns the number of blue bits.
  */
  inline unsigned int getNumberOfBlueBits() const noexcept {
    return blueBits;
  }

  /**
    Returns the number of alpha bits.
  */
  inline unsigned int getNumberOfAlphaBits() const noexcept {
    return alphaBits;
  }

  /**
    Returns true if an accumulator buffer is available.
  */
  inline bool hasAccumulatorBuffer() const noexcept {
    return accumulatorBits;
  }

  /**
    Returns the number of accumulator bits.
  */
  inline unsigned int getNumberOfAcculumatorBits() const noexcept {
    return accumulatorBits;
  }

  /**
    Returns the number of red bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorRedBits() const noexcept {
    return accumulatorRedBits;
  }
  
  /**
    Returns the number of green bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorGreenBits() const noexcept {
    return accumulatorGreenBits;
  }
  
  /**
    Returns the number of blue bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorBlueBits() const noexcept {
    return accumulatorBlueBits;
  }

  /**
    Returns the number of alpha bits for the accumulator buffer.
  */
  inline unsigned int getNumberOfAcculumatorAlphaBits() const noexcept {
    return accumulatorAlphaBits;
  }
  
  /**
    Returns true if depth buffer is available.
  */
  inline bool hasDepthBuffer() const noexcept {
    return depthBits;
  }

  /**
    Returns the number of depth bits.
  */
  inline unsigned int getNumberOfDepthBits() const noexcept {
    return depthBits;
  }

  /**
    Returns true if stencil buffer is available.
  */
  inline bool hasStencilBuffer() const noexcept {
    return stencilBits;
  }

  /**
    Returns the number of stencil bits.
  */
  inline bool getNumberOfStencilBits() const noexcept {
    return stencilBits;
  }

  /**
    Returns true if auxiliary buffers are available.
  */
  inline bool hasAuxBuffers() const noexcept {
    return auxBuffers;
  }
  
  /**
    Returns the number of auxiliary buffers.
  */
  inline unsigned int getNumberOfAuxBuffers() const noexcept {
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
  void deselect() noexcept;

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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
