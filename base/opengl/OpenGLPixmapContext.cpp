/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/opengl/OpenGLPixmapContext.h>
#include <base/platforms/backend/WindowImpl.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/os/win32/GDI.h>
#else // unix
#  include <base/platforms/os/unix/GLX.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

nothing OpenGLPixmapContext::initialize(const Dimension& dimension, unsigned int flags) throw(OpenGLException) {
  screenHandle = 0;
  drawableHandle = 0;
  graphicsContextHandle = 0;
  renderingContextHandle = 0;
  
  OpenGLContextImpl::loadModule();  
  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  drawableHandle = ::CreateWindowEx(
    0, // extended style
    "http://mip.sdu.dk/~fonseca/base/ui/WindowImpl", // class name
    "", // window title
    WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // required for OpenGL - style
    0, // x position
    0, // y position
    dimension.getWidth(),
    dimension.getHeight(),
    0, // no parent window
    0, // use the window class menu
    (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
    0 // application window data structure
  );
  assert(drawableHandle, OpenGLException("Unable to create pixmap", this));
  
  if (!(graphicsContextHandle = ::GetDC((HWND)drawableHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw OpenGLException("Unable to create pixmap", this);
  }
  
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
    1, // version number
    PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL, // support bitmap and OpenGL (rest set below)
    0, // type set below
    24, // 24-bit color depth
    0, 0, 0, 0, 0, 0, // color bits ignored
    0, // no alpha buffer
    0, // shift bit ignored
    0, // accumulation buffer
    0, 0, 0, 0, // accum bits ignored
    0, // z-buffer
    0, // stencil buffer
    0, // auxiliary buffer
    PFD_MAIN_PLANE, // main layer (ignored in new implementations)
    0, // reserved
    0, 0, 0 // layer masks ignored
  };
  
  pfd.dwFlags |= PFD_GENERIC_ACCELERATED; // prefer accelerated
  // pfd.dwFlags |= PFD_DRAW_TO_BITMAP
  // pfd.dwFlags |= PFD_SWAP_LAYER_BUFFERS
  if (flags & OpenGLContextImpl::DOUBLE_BUFFERED) {
    pfd.dwFlags |= PFD_DOUBLEBUFFER;
  }
  if (flags & OpenGLContextImpl::STEREO) {
    pfd.dwFlags |= PFD_STEREO;
  }
  if (flags & OpenGLContextImpl::ACCUMULATOR) {
  }
  if (flags & OpenGLContextImpl::ALPHA) {
  }
  if (flags & OpenGLContextImpl::DEPTH) {
    pfd.cDepthBits = 32;
  }
  if (flags & OpenGLContextImpl::STENCIL) {
    //pfd.cStencilBits = 32;
  }
  if (flags & OpenGLContextImpl::MULTI_SAMPLE) {
  }
  if (flags & OpenGLContextImpl::LUMINANCE) {
  }
  pfd.iPixelType = (flags & OpenGLContextImpl::COLOR_INDEX) ? PFD_TYPE_COLORINDEX : PFD_TYPE_RGBA;    
  
  int index;
  if (!(index = ::ChoosePixelFormat((HDC)graphicsContextHandle, &pfd))) {
    throw OpenGLException("Format not supported", this);
  }

  ::DescribePixelFormat(
    (HDC)graphicsContextHandle,
    index,
    sizeof(PIXELFORMATDESCRIPTOR),
    &pfd
  );

  // TAG: if format is ok (need other checks)
  assert(
    (pfd.dwFlags & PFD_DRAW_TO_BITMAP) &&
    (pfd.dwFlags & PFD_SUPPORT_OPENGL),
    OpenGLException("Format not supported", this)
  );
  
  redBits = pfd.cRedBits;
  redShift = pfd.cRedShift;
  greenBits = pfd.cGreenBits;
  greenShift = pfd.cGreenShift;
  blueBits = pfd.cBlueBits;
  blueShift = pfd.cBlueShift;
  alphaBits = pfd.cAlphaBits;
  alphaShift = pfd.cAlphaShift;
  accumulatorBits = pfd.cAccumBits;
  accumulatorRedBits = pfd.cAccumRedBits;
  accumulatorGreenBits = pfd.cAccumGreenBits;
  accumulatorBlueBits = pfd.cAccumBlueBits;
  accumulatorAlphaBits = pfd.cAccumAlphaBits;
  depthBits = pfd.cDepthBits;
  stencilBits = pfd.cStencilBits;
  auxBuffers = pfd.cAuxBuffers;
  
  numberOfOverlayPlanes = (pfd.bReserved >> 0) & 0xf;
  numberOfUnderlayPlanes = (pfd.bReserved >> 4) & 0xf;
  
  if (!::SetPixelFormat((HDC)graphicsContextHandle, index, &pfd)) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw OpenGLException("Unable to set format", this);
  }
  if (!(renderingContextHandle = openGLContextImpl::wglCreateContext((HDC)graphicsContextHandle))) {
    ::DeleteDC((HDC)graphicsContextHandle);
    graphicsContextHandle = 0;
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw OpenGLException("Unable to create rendering context", this);
  }
  if (!openGLContextImpl::wglMakeCurrent((HDC)graphicsContextHandle, (HGLRC)renderingContextHandle)) {
    ::DeleteDC((HDC)graphicsContextHandle);
    graphicsContextHandle = 0;
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw OpenGLException("Invalid rendering context", this);
  }
#else // unix  
  int screenId = ::XDefaultScreen((Display*)Backend<WindowImpl>::getDisplay());
  screenHandle = ::XScreenOfDisplay((Display*)Backend<WindowImpl>::getDisplay(), screenId);
  assert(screenHandle, UserInterfaceException("Unable to open screen", this));

  XVisualInfo* visualInfo = 0;
  {
    int attributes[33]; // make sure buffer is big enough
    int* dest = attributes;
    *dest++ = GLX_USE_GL;
    *dest++ = GLX_LEVEL;
    *dest++ = 0; // main layer
    if (flags & OpenGLContextImpl::COLOR_INDEX) {
      *dest++ = GLX_BUFFER_SIZE;
      *dest++ = 1; // TAG: fixme
    } else {
      *dest++ = GLX_RGBA;
      *dest++ = GLX_RED_SIZE;
      *dest++ = 1;
      *dest++ = GLX_GREEN_SIZE;
      *dest++ = 1;
      *dest++ = GLX_BLUE_SIZE;
      *dest++ = 1;
    }
    if (flags & OpenGLContextImpl::DOUBLE_BUFFERED) {
      *dest++ = GLX_DOUBLEBUFFER;
    }
    if (flags & OpenGLContextImpl::STEREO) {
      *dest++ = GLX_STEREO;
    }
    if (flags & OpenGLContextImpl::ACCUMULATOR) {
      if (flags & OpenGLContextImpl::COLOR_INDEX) {
      } else {
        *dest++ = GLX_ACCUM_RED_SIZE;
        *dest++ = 1;
        *dest++ = GLX_ACCUM_GREEN_SIZE;
        *dest++ = 1;
        *dest++ = GLX_ACCUM_BLUE_SIZE;
        *dest++ = 1;
      }
    }
    if (flags & OpenGLContextImpl::ALPHA) {
      *dest++ = GLX_ALPHA_SIZE;
      *dest++ = 1;
      if (flags & OpenGLContextImpl::ACCUMULATOR) {
        *dest++ = GLX_ACCUM_ALPHA_SIZE;
        *dest++ = 1;
      }
    }
    if (flags & OpenGLContextImpl::DEPTH) {
      *dest++ = GLX_DEPTH_SIZE;
      *dest++ = 32; // TAG: fixme - minimum value allowed by specification is 12
    } else {
      *dest++ = GLX_DEPTH_SIZE;
      *dest++ = 0;
    }
    if (flags & OpenGLContextImpl::AUX) {
      *dest++ = GLX_AUX_BUFFERS;
      *dest++= 1;
    }
    if (flags & OpenGLContextImpl::STENCIL) {
      *dest++ = GLX_STENCIL_SIZE;
      *dest++ = 1; // TAG: fixme
    }
    if (flags & OpenGLContextImpl::MULTI_SAMPLE) {
    }
    if (flags & OpenGLContextImpl::LUMINANCE) {
    }
    *dest++ = None;
    
    visualInfo = native::GLX::glXChooseVisual(
      (Display*)Backend<WindowImpl>::getDisplay(),
      screenId,
      attributes
    );
    // TAG: screenHandle = 0
    assert(visualInfo, OpenGLException("Format not supported", this));
    
    static const unsigned int ATTRIBUTES[] = {
      GLX_USE_GL,
      GLX_BUFFER_SIZE,
      GLX_LEVEL,
      GLX_RGBA,
      GLX_DOUBLEBUFFER,
      GLX_STEREO,
      GLX_AUX_BUFFERS,
      GLX_RED_SIZE,
      GLX_GREEN_SIZE,
      GLX_BLUE_SIZE,
      GLX_ALPHA_SIZE,
      GLX_DEPTH_SIZE,
      GLX_STENCIL_SIZE,
      GLX_ACCUM_RED_SIZE,
      GLX_ACCUM_GREEN_SIZE,
      GLX_ACCUM_BLUE_SIZE,
      GLX_ACCUM_ALPHA_SIZE
    };
    
    for (unsigned int i = 0; i < getArraySize(ATTRIBUTES); ++i) {
      int value;
      int result = native::GLX::glXGetConfig(
        (Display*)Backend<WindowImpl>::getDisplay(),
        visualInfo,
        ATTRIBUTES[i],
        &value
      );
      if (result == Success) {
        fout << indent(2) << i << ' ' << ATTRIBUTES[i] << ':' << ' ' << value << ENDL;
      }
    }
  }
  
  int blackPixel = ::XBlackPixelOfScreen((Screen*)screenHandle);
  int whitePixel = ::XWhitePixelOfScreen((Screen*)screenHandle);

  // X pixmap
  drawableHandle = (void*)::XCreatePixmap(
    (Display*)Backend<WindowImpl>::getDisplay(),
    DefaultRootWindow((Display*)Backend<WindowImpl>::getDisplay()),
    dimension.getWidth(),
    dimension.getHeight(),
    24 // TAG: fixme
  );
  // detect error and raise exception
  
  renderingContextHandle = (void*)native::GLX::glXCreateGLXPixmap(
    (Display*)Backend<WindowImpl>::getDisplay(),
    visualInfo,
    (Pixmap)drawableHandle
  );
  ::XFree(visualInfo);
  if (renderingContextHandle == 0) {
    ::XFreePixmap((Display*)Backend<WindowImpl>::getDisplay(), (Pixmap)drawableHandle);
    drawableHandle = 0;
    screenHandle = 0;
    throw OpenGLException("Unable to create rendering context", this);
  }
#endif // flavor
  makeCurrent();
  return nothing();
}

OpenGLPixmapContext::OpenGLPixmapContext(const Dimension& dimension, unsigned int flags) throw(OpenGLException)
  : prefixInitialization(initialize(dimension, flags)),
    OpenGLContextImpl() {
}

OpenGLPixmapContext::~OpenGLPixmapContext() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  native::GDI::wglMakeCurrent(0, 0); // deselect current rendering context
  if (renderingContextHandle) {
    native::GDI::wglDeleteContext((HGLRC)renderingContextHandle);
    renderingContextHandle = 0;
  }
  if (graphicsContextHandle) {
    ::DeleteDC((HDC)graphicsContextHandle);
    graphicsContextHandle = 0;
  }
  if (drawableHandle) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
  }
  // screenHandle not used
#else // unix
  native::GLX::glXMakeCurrent(
    (Display*)Backend<WindowImpl>::getDisplay(),
    (GLXDrawable)None,
    (GLXContext)0
  );
  if (renderingContextHandle) {
    native::GLX::glXDestroyGLXPixmap(
      (Display*)Backend<WindowImpl>::getDisplay(),
      (GLXPixmap)renderingContextHandle
    );
    renderingContextHandle = 0;
  }
  // graphicsContextHandle not used
  if (drawableHandle) {
    ::XFreePixmap((Display*)Backend<WindowImpl>::getDisplay(), (Pixmap)drawableHandle);
    drawableHandle = 0;
  }
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
