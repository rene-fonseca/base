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
#include <base/opengl/OpenGLContext.h>
#include <base/platforms/backend/WindowImpl.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix
#  include <base/platforms/os/unix/GLX.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void OpenGLContext::destroy() throw() {
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
  }
  // screenHandle not used
#else // unix
  native::GLX::glXMakeCurrent(
    (Display*)Backend<WindowImpl>::getDisplay(),
    (GLXDrawable)None,
    (GLXContext)0
  );
  if (renderingContextHandle) {
    native::GLX::glXDestroyContext((Display*)displayHandle, (GLXContext)renderingContextHandle);
    renderingContextHandle = 0;
  }
  // graphicsContextHandle
  if (drawableHandle) {
    // GLX 1.3 native::GLX::glXDestroyWindow((Display*)displayHandle, (GLXWindow)drawableHandle);
    ::XDestroyWindow((Display*)displayHandle, (::Window)drawableHandle);
  }
  // screenHandle
#endif // flavor
  WindowImpl::destroy();
}

nothing OpenGLContext::initialize() throw(UserInterfaceException) {
  OpenGLContextImpl::loadModule();
  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = position.getX();
  rect.right = position.getX() + dimension.getWidth();
  rect.top = position.getY();
  rect.bottom = position.getY() + dimension.getHeight();
  DWORD extendedStyle = 0;
  DWORD style = 0;
  
  if (false) { // displayMode == MODE_FULL_SCREEN) {
    extendedStyle = WS_EX_APPWINDOW;
    style = WS_POPUP;
    // TAG: not here ::ShowCursor(FALSE);
  } else {
    extendedStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    style = WS_OVERLAPPEDWINDOW;
  }
  ::AdjustWindowRectEx(&rect, style, FALSE, extendedStyle);
  
  drawableHandle = ::CreateWindowEx(
    extendedStyle, // extended style
    "http://mip.sdu.dk/~fonseca/base/ui/WindowImpl", // class name
    "", // window title
    style | // window style
    WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // required for OpenGL
    rect.left, // x position,
    rect.top, // y position,
    rect.right - rect.left,
    rect.bottom - rect.top,
    0, // no parent window
    0, // use the window class menu
    (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
    0 // application window data structure
  );
  assert(drawableHandle, UserInterfaceException("Unable to create window", this));
  
  if (!(graphicsContextHandle = ::GetDC((HWND)drawableHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw UserInterfaceException("Unable to connect to device context", this);
  }
  
  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
    1, // version number
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL, // support window and OpenGL (rest set below)
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
  if (flags & OpenGLContext::DOUBLE_BUFFERED) {
    pfd.dwFlags |= PFD_DOUBLEBUFFER;
  }
  if (flags & OpenGLContext::STEREO) {
    pfd.dwFlags |= PFD_STEREO;
  }
  if (flags & OpenGLContext::ACCUMULATOR) {
    // TAG: fixme
  }
  if (flags & OpenGLContext::ALPHA) {
    // TAG: fixme
  }
  if (flags & OpenGLContext::DEPTH) {
    // TAG: fixme
    pfd.cDepthBits = 32;
  }
  if (flags & OpenGLContext::STENCIL) {
    // TAG: fixme
    //pfd.cStencilBits = 32;
  }
  if (flags & OpenGLContext::MULTI_SAMPLE) {
    // TAG: fixme
  }
  if (flags & OpenGLContext::LUMINANCE) {
    // TAG: fixme
  }
  pfd.iPixelType = (flags & OpenGLContext::COLOR_INDEX) ? PFD_TYPE_COLORINDEX : PFD_TYPE_RGBA;    
  
  int index;
  if (!(index = ::ChoosePixelFormat((HDC)graphicsContextHandle, &pfd))) {
    throw UserInterfaceException("Format not supported", this);
  }

  ::DescribePixelFormat(
    (HDC)graphicsContextHandle,
    index,
    sizeof(PIXELFORMATDESCRIPTOR),
    &pfd
  );

  // TAG: if format is ok (need other checks)
  assert(
    (pfd.dwFlags & PFD_DRAW_TO_WINDOW) &&
    (pfd.dwFlags & PFD_SUPPORT_OPENGL),
    UserInterfaceException("Format not supported", this)
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
    throw UserInterfaceException("Unable to set format", this);
  }
  if (!(renderingContextHandle = native::GDI::wglCreateContext((HDC)graphicsContextHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    throw UserInterfaceException("Unable to create rendering context", this);
  }
  if (!native::GDI::wglMakeCurrent((HDC)graphicsContextHandle, (HGLRC)renderingContextHandle)) {
    ::DestroyWindow((HWND)drawableHandle);
    throw UserInterfaceException("Invalid rendering context", this);
  }
#else // unix
  {
    int errorBase;
    int eventBase;
    Bool result = native::GLX::glXQueryExtension((Display*)displayHandle, &errorBase, &eventBase);
    assert(result == True, OpenGLException("OpenGL not supported", this));
    
    int major;
    int minor;
    result = native::GLX::glXQueryVersion((Display*)displayHandle, &major, &minor);
    version = minimum(major, 255) << 16 | minimum(minor, 255) << 8;
  }
  
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  assert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
//   {
//     // GLX 1.3
//     int numberOfConfigs;
//     GLXFBConfig* configs = native::GLX::glXGetFBConfigs((Display*)displayHandle, screenId, &numberOfConfigs);

//     static const unsigned int ATTRIBUTES[] = {
//       GLX_X_VISUAL_TYPE,
//       GLX_CONFIG_CAVEAT,
//       GLX_TRANSPARENT_TYPE,
//       GLX_TRANSPARENT_INDEX_VALUE,
//       GLX_TRANSPARENT_RED_VALUE,
//       GLX_TRANSPARENT_GREEN_VALUE,
//       GLX_TRANSPARENT_BLUE_VALUE,
//       GLX_TRANSPARENT_ALPHA_VALUE,
//       GLX_DRAWABLE_TYPE,
//       GLX_RENDER_TYPE,
//       GLX_X_RENDERABLE,
//       GLX_FBCONFIG_ID,
//       GLX_MAX_PBUFFER_WIDTH,
//       GLX_MAX_PBUFFER_HEIGHT,
//       GLX_MAX_PBUFFER_PIXELS,
//       GLX_VISUAL_ID
//     };
    
//     for (unsigned int i = 0; i < numberOfConfigs; ++i) {
//       fout << MESSAGE("Config: ") << i << ENDL;
//       for (unsigned int j = 0; j < getArraySize(ATTRIBUTES); ++j) {
//         int value;
//         int result = native::GLX::glXGetFBConfigAttrib((Display*)displayHandle, configs[i], ATTRIBUTES[j], &value);
//         if (result == Success) {
//           fout << indent(2) << ATTRIBUTES[j] << ':' << ' ' << value << ENDL;
//         }
//       }
//     }
    
//     ::XFree(configs);
//   }

  XVisualInfo* visualInfo = 0;
  {
    int attributes[33]; // make sure buffer is big enough
    int* dest = attributes;
    *dest++ = GLX_USE_GL;
    *dest++ = GLX_LEVEL;
    *dest++ = 0; // main layer
    if (flags & OpenGLContext::COLOR_INDEX) {
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
    if (flags & OpenGLContext::DOUBLE_BUFFERED) {
      *dest++ = GLX_DOUBLEBUFFER;
    }
    if (flags & OpenGLContext::STEREO) {
      *dest++ = GLX_STEREO;
    }
    if (flags & OpenGLContext::ACCUMULATOR) {
      if (flags & OpenGLContext::COLOR_INDEX) {
      } else {
        *dest++ = GLX_ACCUM_RED_SIZE;
        *dest++ = 1;
        *dest++ = GLX_ACCUM_GREEN_SIZE;
        *dest++ = 1;
        *dest++ = GLX_ACCUM_BLUE_SIZE;
        *dest++ = 1;
      }
    }
    if (flags & OpenGLContext::ALPHA) {
      *dest++ = GLX_ALPHA_SIZE;
      *dest++ = 1;
      if (flags & OpenGLContext::ACCUMULATOR) {
        *dest++ = GLX_ACCUM_ALPHA_SIZE;
        *dest++ = 1;
      }
    }
    if (flags & OpenGLContext::DEPTH) {
      *dest++ = GLX_DEPTH_SIZE;
      *dest++ = 32; // TAG: fixme - minimum value allowed by specification is 12
    } else {
      *dest++ = GLX_DEPTH_SIZE;
      *dest++ = 0;
    }
    if (flags & OpenGLContext::AUX) {
      *dest++ = GLX_AUX_BUFFERS;
      *dest++= 1;
    }
    if (flags & OpenGLContext::STENCIL) {
      *dest++ = GLX_STENCIL_SIZE;
      *dest++ = 1; // TAG: fixme
    }
    if (flags & OpenGLContext::MULTI_SAMPLE) {
    }
    if (flags & OpenGLContext::LUMINANCE) {
    }
    *dest++ = None;
    
    visualInfo = native::GLX::glXChooseVisual(
      (Display*)displayHandle,
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
        (Display*)displayHandle,
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

  // GLX 1.3
  // GLXWindow gLXWindow = native::GLX::glXCreateWindow((Display*)displayHandle, GLXFBConfig config, ::Window win, 0);
  
  drawableHandle = (void*)::XCreateSimpleWindow(
    (Display*)displayHandle,
    DefaultRootWindow((Display*)displayHandle),
    position.getX(),
    position.getY(),
    dimension.getWidth(),
    dimension.getHeight(),
    0, // border width
    blackPixel, // border color
    blackPixel // background
  );
  // detect error and raise exception
  
  renderingContextHandle = native::GLX::glXCreateContext(
    (Display*)displayHandle,
    visualInfo,
    0,
    (flags & OpenGLContext::DIRECT) ? True : False
  );
  ::XFree(visualInfo);
  if (renderingContextHandle == 0) {
    ::XDestroyWindow((Display*)displayHandle, (::Window)drawableHandle);
    drawableHandle = 0;
    screenHandle = 0;
    throw OpenGLException("Unable to create rendering context", this);
  }
  makeCurrent();
  
  ::XSelectInput(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    StructureNotifyMask |
    FocusChangeMask |
    VisibilityChangeMask |
    KeyPressMask |
    KeyReleaseMask |
    KeymapStateMask |
    ButtonPressMask |
    ButtonReleaseMask |
    EnterWindowMask |
    LeaveWindowMask |
    PointerMotionMask
  );
#endif // flavor
  return nothing();
}

OpenGLContext::OpenGLContext(
  const Position& position,
  const Dimension& dimension,
  unsigned int flags) throw(OpenGLException, UserInterfaceException)
  : WindowImpl(position, dimension, 0), // TAG: fix flags
    prefixInitialization(initialize()),
    OpenGLContextImpl() {
  construct();
  invalidate();
}

void OpenGLContext::onDisplay() throw() {
  openGL.glClearColor(0.0, 0.0, 0.0, 1.0);
  openGL.glClear(OpenGL::COLOR_BUFFER_BIT);
  openGL.glFlush();
  swap();
}

void OpenGLContext::onResize(const Dimension& dimension) throw() {
  openGL.glViewport(0, 0, dimension.getWidth(), dimension.getHeight());
  onDisplay();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
