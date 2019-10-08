/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/opengl/OpenGLContext.h>
#include <base/platforms/backend/WindowImpl.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/win32/GDI.h>
#else // unix
#if 0 && (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
#else
#  include <base/platforms/os/unix/GLX.h>
#endif
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void OpenGLContext::destroy() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  native::GDI::wglMakeCurrent(0, 0); // deselect current rendering context
  if (renderingContextHandle) {
    native::GDI::wglDeleteContext((HGLRC)renderingContextHandle);
    renderingContextHandle = 0;
  }
  if (WindowImpl::graphicsContextHandle) {
    ::DeleteDC((HDC)WindowImpl::graphicsContextHandle);
  }
  if (drawableHandle) {
    // nothing to destroy
  }
  // screenHandle not used
#else // unix
  native::GLX::glXMakeCurrent(
    (Display*)Backend<WindowImpl>::getDisplay(),
    (native::GLX::GLXDrawable)None,
    (native::GLX::GLXContext)0
  );
  if (renderingContextHandle) {
    native::GLX::glXDestroyContext(
      (Display*)displayHandle,
      (native::GLX::GLXContext)renderingContextHandle
    );
    renderingContextHandle = 0;
  }
  if (WindowImpl::graphicsContextHandle) {
    // nothing to destroy
  }
  if (drawableHandle) {
    // GLX 1.3 native::GLX::glXDestroyWindow((Display*)displayHandle, (native::GLX::GLXWindow)drawableHandle);
    ::XDestroyWindow((Display*)displayHandle, (::Window)drawableHandle);
  }
  if (screenHandle) {
    // nothing to destroy
  }
#endif // flavor
  WindowImpl::destroy();
}

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
struct MonitorEnumeratorData {
  Array<OpenGLContext::Format> formats;
  unsigned int flags;
  unsigned int id;
};

BOOL CALLBACK enumerateMonitor(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM context) throw() {
  MonitorEnumeratorData* temp = (MonitorEnumeratorData*)context;
  int numberOfFormats = ::DescribePixelFormat(hdc, 0, 0, 0);
  
  for (int i = 1; i <= numberOfFormats; ++i) {
    PIXELFORMATDESCRIPTOR pfd;
    ::DescribePixelFormat(hdc, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    
    bool candidate = (pfd.dwFlags & PFD_DRAW_TO_WINDOW) && (pfd.dwFlags & PFD_SUPPORT_OPENGL);
    if (temp->flags & OpenGLContext::COLOR_INDEXED) {
      candidate &= pfd.iPixelType == PFD_TYPE_COLORINDEX;
    }
    if (temp->flags & OpenGLContext::RGB) {
      candidate &= pfd.iPixelType == PFD_TYPE_RGBA;
    }
    if (temp->flags & OpenGLContext::DOUBLE_BUFFERED) {
      candidate &= (pfd.dwFlags & PFD_DOUBLEBUFFER) != 0;
    }
    if (temp->flags & OpenGLContext::STEREO) {
      candidate &= (pfd.dwFlags & PFD_STEREO) != 0;
    }
    if (temp->flags & OpenGLContext::DIRECT) {
      candidate &= (pfd.dwFlags & PFD_GENERIC_FORMAT) == 0;
    }
    if (temp->flags & OpenGLContext::GENERIC) {
      candidate &= (pfd.dwFlags & PFD_GENERIC_FORMAT) != 0;
    }
    if (temp->flags & OpenGLContext::ALPHA) {
      candidate &= pfd.cAlphaBits > 0;
    }
    if (temp->flags & OpenGLContext::ACCUMULATOR) {
      candidate &= pfd.cAccumBits > 0;
    }
    if (temp->flags & OpenGLContext::DEPTH) {
      candidate &= pfd.cDepthBits > 0;
    }
    if (temp->flags & OpenGLContext::STENCIL) {
      candidate &= pfd.cStencilBits > 0;
    }
    if (temp->flags & OpenGLContext::AUX) {
      candidate &= pfd.cAuxBuffers > 0;
    }
    if (candidate) {
      OpenGLContext::Format format;
      format.id = temp->id++;
      format.flags = 0;
      if (pfd.iPixelType == PFD_TYPE_COLORINDEX) {
        format.flags |= OpenGLContext::COLOR_INDEXED;
      }
      if (pfd.iPixelType == PFD_TYPE_RGBA) {
        format.flags |= OpenGLContext::RGB;
      }
      if (pfd.dwFlags & PFD_GENERIC_FORMAT) {
        format.flags |= OpenGLContext::GENERIC;
      }
      if (pfd.dwFlags & PFD_DOUBLEBUFFER) {
        format.flags |= OpenGLContext::DOUBLE_BUFFERED;
      }
      if (pfd.dwFlags & PFD_STEREO) {
        format.flags |= OpenGLContext::STEREO;
      }
      format.colorBits = pfd.cColorBits;
      format.redBits = pfd.cRedBits;
      format.greenBits = pfd.cGreenBits;
      format.blueBits = pfd.cBlueBits;
      format.alphaBits = pfd.cAlphaBits;
      format.accumulatorBits = pfd.cAccumBits;
      format.accumulatorRedBits = pfd.cAccumRedBits;
      format.accumulatorGreenBits = pfd.cAccumGreenBits;
      format.accumulatorBlueBits = pfd.cAccumBlueBits;
      format.accumulatorAlphaBits = pfd.cAccumAlphaBits;
      format.depthBits = pfd.cDepthBits;
      format.stencilBits = pfd.cStencilBits;
      format.auxBuffers = pfd.cAuxBuffers;
      temp->formats.append(format);
    }
  }
  return TRUE;
}
#endif // flavor

Array<OpenGLContext::Format> OpenGLContext::getFormats(unsigned int flags) throw(OpenGLException, UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  MonitorEnumeratorData temp;
  temp.flags = flags;
  temp.id = 0;
  HDC hdc = ::GetDC(0); // GetDCEx(0, 0, 0);
  ::EnumDisplayMonitors(hdc, 0, enumerateMonitor, (LPARAM)&temp);
  ::ReleaseDC(0, hdc);
  return temp.formats;
#else // unix
  Array<OpenGLContext::Format> formats;
  return formats;
#endif // flavor
}

nothing OpenGLContext::initialize(const Format& format) throw(OpenGLException, UserInterfaceException) {
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
    _DK_SDU_MIP__BASE__ID_PREFIX L"/ui/WindowImpl", // class name
    L"", // window title
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
  bassert(drawableHandle, UserInterfaceException("Unable to create window", this));
  
  if (!(WindowImpl::graphicsContextHandle = ::GetDC((HWND)drawableHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw UserInterfaceException("Unable to connect to device context", this);
  }
  OpenGLContextImpl::graphicsContextHandle = WindowImpl::graphicsContextHandle; // TAG: fixme

  int formatId = 0;
  int numberOfFormats = ::DescribePixelFormat((HDC)WindowImpl::graphicsContextHandle, 0, 0, 0);
  PIXELFORMATDESCRIPTOR pfd;
  for (int i = 1; i <= numberOfFormats; ++i) {
    ::DescribePixelFormat(
      (HDC)WindowImpl::graphicsContextHandle,
      i,
      sizeof(PIXELFORMATDESCRIPTOR),
      &pfd
    );
    
    bool candidate = (pfd.dwFlags & PFD_DRAW_TO_WINDOW) && (pfd.dwFlags & PFD_SUPPORT_OPENGL);
    if (format.flags & OpenGLContext::COLOR_INDEXED) {
      candidate &= pfd.iPixelType == PFD_TYPE_COLORINDEX;
    }
    if (format.flags & OpenGLContext::RGB) {
      candidate &= pfd.iPixelType == PFD_TYPE_RGBA;
    }
    if (format.flags & OpenGLContext::GENERIC) {
      candidate &= (pfd.dwFlags & PFD_GENERIC_FORMAT) != 0;
    }
    if (format.flags & OpenGLContext::DOUBLE_BUFFERED) {
      candidate &= (pfd.dwFlags & PFD_DOUBLEBUFFER) != 0;
    }
    if (format.flags & OpenGLContext::STEREO) {
      candidate &= (pfd.dwFlags & PFD_STEREO) != 0;
    }
    candidate &= pfd.cColorBits == format.colorBits;
    candidate &= pfd.cRedBits == format.redBits;
    candidate &= pfd.cGreenBits == format.greenBits;
    candidate &= pfd.cBlueBits == format.blueBits;
    candidate &= pfd.cAlphaBits == format.alphaBits;
    candidate &= pfd.cAccumBits == format.accumulatorBits;
    candidate &= pfd.cAccumRedBits == format.accumulatorRedBits;
    candidate &= pfd.cAccumGreenBits == format.accumulatorGreenBits;
    candidate &= pfd.cAccumBlueBits == format.accumulatorBlueBits;
    candidate &= pfd.cAccumAlphaBits == format.accumulatorAlphaBits;
    candidate &= pfd.cDepthBits == format.depthBits;
    candidate &= pfd.cStencilBits == format.stencilBits;
    candidate &= pfd.cAuxBuffers == format.auxBuffers;
    if (candidate) {
      formatId = i;
      break;
    }
  }
  bassert(formatId != 0, OpenGLException("Requested format not supported", this));
  
  OpenGLContextImpl::flags = 0;
  OpenGLContextImpl::flags |= (pfd.iPixelType == PFD_TYPE_COLORINDEX) ? OpenGLContext::COLOR_INDEXED : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_DOUBLEBUFFER) ? OpenGLContext::DOUBLE_BUFFERED : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_STEREO) ? OpenGLContext::STEREO : 0;
  // OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_SWAP_LAYER_BUFFERS) ? OpenGLContext::SWAP_LAYER : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_GENERIC_ACCELERATED) ? OpenGLContext::DIRECT : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_GENERIC_FORMAT) ? OpenGLContext::GENERIC : 0;
  
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
  
  if (!::SetPixelFormat((HDC)WindowImpl::graphicsContextHandle, formatId, &pfd)) {
    throw OpenGLException("Unable to set format", this);
  }
  if (!(renderingContextHandle = native::GDI::wglCreateContext(
          (HDC)WindowImpl::graphicsContextHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw OpenGLException("Unable to create rendering context", this);
  }
  if (!native::GDI::wglMakeCurrent(
        (HDC)WindowImpl::graphicsContextHandle, (HGLRC)renderingContextHandle)) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw OpenGLException("Invalid rendering context", this);
  }
#else // unix
  fout << "GLX version: " << HEX << native::GLX::version << ENDL;
  
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  bassert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
//   {
//     // GLX 1.3
//     int numberOfConfigs;
//     GLXFBConfig* configs = native::GLX::glXGetFBConfigs((Display*)displayHandle, screenId, &numberOfConfigs);

//     static const unsigned int ATTRIBUTES[] = {
//       native::GLX::X_VISUAL_TYPE,
//       native::GLX::CONFIG_CAVEAT,
//       native::GLX::TRANSPARENT_TYPE,
//       native::GLX::TRANSPARENT_INDEX_VALUE,
//       native::GLX::TRANSPARENT_RED_VALUE,
//       native::GLX::TRANSPARENT_GREEN_VALUE,
//       native::GLX::TRANSPARENT_BLUE_VALUE,
//       native::GLX::TRANSPARENT_ALPHA_VALUE,
//       native::GLX::DRAWABLE_TYPE,
//       native::GLX::RENDER_TYPE,
//       native::GLX::X_RENDERABLE,
//       native::GLX::FBCONFIG_ID,
//       native::GLX::MAX_PBUFFER_WIDTH,
//       native::GLX::MAX_PBUFFER_HEIGHT,
//       native::GLX::MAX_PBUFFER_PIXELS,
//       native::GLX::VISUAL_ID
//     };
    
//     for (unsigned int i = 0; i < numberOfConfigs; ++i) {
//       fout << "Config: " << i << ENDL;
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
//     *dest++ = native::GLX::USE_GL;
//     *dest++ = native::GLX::LEVEL;
//     *dest++ = 0; // main layer
    if (WindowImpl::flags & OpenGLContext::COLOR_INDEXED) {
//       *dest++ = native::GLX::BUFFER_SIZE;
//       *dest++ = 1; // TAG: fixme
    } else {
      *dest++ = native::GLX::RGBA;
//       *dest++ = native::GLX::RED_SIZE;
//       *dest++ = 1;
//       *dest++ = native::GLX::GREEN_SIZE;
//       *dest++ = 1;
//       *dest++ = native::GLX::BLUE_SIZE;
//       *dest++ = 1;
    }
    if (WindowImpl::flags & OpenGLContext::DOUBLE_BUFFERED) {
      *dest++ = native::GLX::DOUBLEBUFFER;
    }
//     if (WindowImpl::flags & OpenGLContext::STEREO) {
//       *dest++ = native::GLX::STEREO;
//     }
//     if (WindowImpl::flags & OpenGLContext::ACCUMULATOR) {
//       if (WindowImpl::flags & OpenGLContext::COLOR_INDEXED) {
//       } else {
//         *dest++ = native::GLX::ACCUM_RED_SIZE;
//         *dest++ = 1;
//         *dest++ = native::GLX::ACCUM_GREEN_SIZE;
//         *dest++ = 1;
//         *dest++ = native::GLX::ACCUM_BLUE_SIZE;
//         *dest++ = 1;
//       }
//     }
//     if (WindowImpl::flags & OpenGLContext::ALPHA) {
//       *dest++ = native::GLX::ALPHA_SIZE;
//       *dest++ = 1;
//       if (WindowImpl::flags & OpenGLContext::ACCUMULATOR) {
//         *dest++ = native::GLX::ACCUM_ALPHA_SIZE;
//         *dest++ = 1;
//       }
//     }
//     if (WindowImpl::flags & OpenGLContext::DEPTH) {
//       *dest++ = native::GLX::DEPTH_SIZE;
//       *dest++ = 12; // TAG: fixme - minimum value allowed by specification is 12
//     } else {
//       *dest++ = native::GLX::DEPTH_SIZE;
//       *dest++ = 0;
//     }
//     if (WindowImpl::flags & OpenGLContext::AUX) {
//       *dest++ = native::GLX::AUX_BUFFERS;
//       *dest++= 1;
//     }
//     if (WindowImpl::flags & OpenGLContext::STENCIL) {
//       *dest++ = native::GLX::STENCIL_SIZE;
//       *dest++ = 1; // TAG: fixme
//     }
//     if (WindowImpl::flags & OpenGLContext::MULTI_SAMPLE) {
//     }
//     if (WindowImpl::flags & OpenGLContext::LUMINANCE) {
//     }
    *dest++ = None;
    
    static int attributeList[] = {native::GLX::RGBA, None};
    //static int ATTRIBUTES2[] = {native::GLX::RGBA, native::GLX::RED_SIZE, 4, native::GLX::GREEN_SIZE, 4, native::GLX::BLUE_SIZE, 4, None};
    visualInfo = native::GLX::glXChooseVisual(
      (Display*)displayHandle,
      screenId,
      attributeList // attributes
    );
    // TAG: screenHandle = 0
    bassert(visualInfo, OpenGLException("Format not supported", this));

    renderingContextHandle = native::GLX::glXCreateContext(
    (Display*)displayHandle,
    visualInfo,
    0,
    (WindowImpl::flags & OpenGLContext::DIRECT) ? True : False
  );
  
    static const unsigned int ATTRIBUTES[] = {
      native::GLX::USE_GL,
      native::GLX::BUFFER_SIZE,
      native::GLX::LEVEL,
      native::GLX::RGBA,
      native::GLX::DOUBLEBUFFER,
      native::GLX::STEREO,
      native::GLX::AUX_BUFFERS,
      native::GLX::RED_SIZE,
      native::GLX::GREEN_SIZE,
      native::GLX::BLUE_SIZE,
      native::GLX::ALPHA_SIZE,
      native::GLX::DEPTH_SIZE,
      native::GLX::STENCIL_SIZE,
      native::GLX::ACCUM_RED_SIZE,
      native::GLX::ACCUM_GREEN_SIZE,
      native::GLX::ACCUM_BLUE_SIZE,
      native::GLX::ACCUM_ALPHA_SIZE
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
  construct();
  invalidate();
  update();
  return nothing();
}

OpenGLContext::OpenGLContext(
  const Position& position,
  const Dimension& dimension,
  const Format& format) throw(OpenGLException, UserInterfaceException)
  : WindowImpl(position, dimension, 0), // TAG: fix flags
    OpenGLContextImpl(),
    prefixInitialization(initialize(format)) {
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
