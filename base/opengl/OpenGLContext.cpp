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
#include <base/Application.h>
#include <base/dl/DynamicLinker.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (glx)
#  include <GL/glx.h>
#endif // flavor

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_OPENGL _DK_SDU_MIP__BASE__CALL_PASCAL
#else
#  define CALL_OPENGL
#endif

// TAG: put in static description
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define _DK_SDU_MIP__BASE__OPENGL_LIBRARY "OPENGL32.DLL"
#else // win32
#  define _DK_SDU_MIP__BASE__OPENGL_LIBRARY "libGL.so"
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace openGLContextImpl {

  DynamicLinker* dynamicLinker = 0;

  typedef void (CALL_OPENGL *OpenGLFunction)();
  
  struct Descriptor {
    const StringLiteral symbol;
    OpenGLFunction* function;
  };
  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  
  enum {
    WGL_NUMBER_PIXEL_FORMATS_ARB = 0x2000,
    WGL_DRAW_TO_WINDOW_ARB = 0x2001,
    WGL_DRAW_TO_BITMAP_ARB = 0x2002,
    WGL_ACCELERATION_ARB = 0x2003,
    WGL_NEED_PALETTE_ARB = 0x2004,
    WGL_NEED_SYSTEM_PALETTE_ARB = 0x2005,
    WGL_SWAP_LAYER_BUFFERS_ARB = 0x2006,
    WGL_SWAP_METHOD_ARB = 0x2007,
    WGL_NUMBER_OVERLAYS_ARB = 0x2008,
    WGL_NUMBER_UNDERLAYS_ARB = 0x2009,
    WGL_TRANSPARENT_ARB = 0x200A,
    WGL_TRANSPARENT_RED_VALUE_ARB = 0x2037,
    WGL_TRANSPARENT_GREEN_VALUE_ARB = 0x2038,
    WGL_TRANSPARENT_BLUE_VALUE_ARB = 0x2039,
    WGL_TRANSPARENT_ALPHA_VALUE_ARB = 0x203a,
    WGL_TRANSPARENT_INDEX_VALUE_ARB = 0x203b,
    WGL_SHARE_DEPTH_ARB = 0x200c,
    WGL_SHARE_STENCIL_ARB = 0x200d,
    WGL_SHARE_ACCUM_ARB = 0x200e,
    WGL_SUPPORT_GDI_ARB = 0x200f,
    WGL_SUPPORT_OPENGL_ARB = 0x2010,
    WGL_DOUBLE_BUFFER_ARB = 0x2011,
    WGL_STEREO_ARB = 0x2012,
    WGL_PIXEL_TYPE_ARB = 0x2013,
    WGL_COLOR_BITS_ARB = 0x2014,
    WGL_RED_BITS_ARB = 0x2015,
    WGL_RED_SHIFT_ARB = 0x2016,
    WGL_GREEN_BITS_ARB = 0x2017,
    WGL_GREEN_SHIFT_ARB = 0x2018,
    WGL_BLUE_BITS_ARB = 0x2019,
    WGL_BLUE_SHIFT_ARB = 0x201a,
    WGL_ALPHA_BITS_ARB = 0x201b,
    WGL_ALPHA_SHIFT_ARB = 0x201c,
    WGL_ACCUM_BITS_ARB = 0x201d,
    WGL_ACCUM_RED_BITS_ARB = 0x201e,
    WGL_ACCUM_GREEN_BITS_ARB = 0x201f,
    WGL_ACCUM_BLUE_BITS_ARB = 0x2020,
    WGL_ACCUM_ALPHA_BITS_ARB = 0x2021,
    WGL_DEPTH_BITS_ARB = 0x2022,
    WGL_STENCIL_BITS_ARB = 0x2023,
    WGL_AUX_BUFFERS_ARB = 0x2024,

    WGL_NO_ACCELERATION_ARB = 0x2025,
    WGL_GENERIC_ACCELERATION_ARB = 0x2026,
    WGL_FULL_ACCELERATION_ARB = 0x2027,
    WGL_SWAP_EXCHANGE_ARB = 0x2028,
    WGL_SWAP_COPY_ARB = 0x2029,
    WGL_SWAP_UNDEFINED_ARB = 0x202a,
    WGL_TYPE_RGBA_ARB = 0x202b,
    WGL_TYPE_COLORINDEX_ARB = 0x202c
  };
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  
  HGLRC (CALL_OPENGL *wglCreateContext)(HDC hdc);
  HGLRC (CALL_OPENGL *wglCreateLayerContext)(HDC hdc, int plane);
  BOOL (CALL_OPENGL *wglDeleteContext)(HGLRC hglrc);
  HGLRC (CALL_OPENGL *wglGetCurrentContext)();
  HDC (CALL_OPENGL *wglGetCurrentDC)();
  PROC (CALL_OPENGL *wglGetProcAddress)(LPCSTR lpszProc);
  BOOL (CALL_OPENGL *wglMakeCurrent)(HDC hdc, HGLRC hglrc);
  BOOL (CALL_OPENGL *wglSwapLayerBuffers)(HDC hdc, UINT planes);
  BOOL (CALL_OPENGL *wglDescribeLayerPlane)(HDC hdc, int pixelFormat, int layerPlane, UINT bytes, LPLAYERPLANEDESCRIPTOR lpd);
  
  static Descriptor OPEN_GL_FUNCTIONS[] = {
    {MESSAGE("wglCreateContext"), Cast::pointer<OpenGLFunction*>((void*)&wglCreateContext)},
    {MESSAGE("wglCreateLayerContext"), Cast::pointer<OpenGLFunction*>((void*)&wglCreateLayerContext)},
    {MESSAGE("wglDeleteContext"), Cast::pointer<OpenGLFunction*>((void*)&wglDeleteContext)},
    {MESSAGE("wglGetCurrentContext"), Cast::pointer<OpenGLFunction*>((void*)&wglGetCurrentContext)},
    {MESSAGE("wglGetCurrentDC"), Cast::pointer<OpenGLFunction*>((void*)&wglGetCurrentDC)},
    {MESSAGE("wglGetProcAddress"), Cast::pointer<OpenGLFunction*>((void*)&wglGetProcAddress)},
    {MESSAGE("wglMakeCurrent"), Cast::pointer<OpenGLFunction*>((void*)&wglMakeCurrent)},
    {MESSAGE("wglSwapLayerBuffers"), Cast::pointer<OpenGLFunction*>((void*)&wglSwapLayerBuffers)},
    {MESSAGE("wglDescribeLayerPlane"), Cast::pointer<OpenGLFunction*>((void*)&wglDescribeLayerPlane)}
  };
#else // unix

  // GLX 1.0
  XVisualInfo* (CALL_OPENGL *glXChooseVisual)(Display* dpy, int screen, int* attribList);
  void (CALL_OPENGL *glXCopyContext)(Display* dpy, GLXContext src, GLXContext dst, GLuint mask);
  GLXContext (CALL_OPENGL *glXCreateContext)(Display* dpy, XVisualInfo* vis, GLXContext shareList, Bool direct);
  GLXPixmap (CALL_OPENGL *glXCreateGLXPixmap)(Display* dpy, XVisualInfo* vis, Pixmap pixmap);
  void (CALL_OPENGL *glXDestroyContext)(Display* dpy, GLXContext ctx);
  void (CALL_OPENGL *glXDestroyGLXPixmap)(Display* dpy, GLXPixmap pix);
  int (CALL_OPENGL *glXGetConfig)(Display* dpy, XVisualInfo* vis, int attrib, int* value);
  GLXContext (CALL_OPENGL *glXGetCurrentContext)();
  GLXDrawable (CALL_OPENGL *glXGetCurrentDrawable)();
  Bool (CALL_OPENGL *glXIsDirect)(Display* dpy, GLXContext ctx);
  Bool (CALL_OPENGL *glXMakeCurrent)(Display* dpy, GLXDrawable drawable, GLXContext ctx);
  Bool (CALL_OPENGL *glXQueryExtension)(Display* dpy, int* errorBase, int* eventBase);
  Bool (CALL_OPENGL *glXQueryVersion)(Display* dpy, int* major, int* minor);
  void (CALL_OPENGL *glXSwapBuffers)(Display* dpy, GLXDrawable drawable);
  void (CALL_OPENGL *glXUseXFont)(Font font, int first, int count, int listBase);
  void (CALL_OPENGL *glXWaitGL)();
  void (CALL_OPENGL *glXWaitX)();
  
  // GLX 1.1
  const char* (CALL_OPENGL *glXQueryExtensionsString)(Display* dpy, int screen);
  const char* (CALL_OPENGL *glXGetClientString)(Display* dpy, int name);
  const char* (CALL_OPENGL *glXQueryServerString)(Display* dpy, int screen, int name);

  // GLX 1.3
  GLXFBConfig* (CALL_OPENGL *glXGetFBConfigs)(Display* dpy, int screen, int* nelements);
  GLXFBConfig* (CALL_OPENGL *glXChooseFBConfig)(Display* dpy, int screen, const int* attrib_list, int* nelements);
  int (CALL_OPENGL *glXGetFBConfigAttrib)(Display* dpy, GLXFBConfig config, int attribute, int* value);
  XVisualInfo* (CALL_OPENGL *glXGetVisualFromFBConfig)(Display* dpy, GLXFBConfig config);
  GLXWindow (CALL_OPENGL *glXCreateWindow)(Display* dpy, GLXFBConfig config, Window win, const int* attrib_list);
  void (CALL_OPENGL *glXDestroyWindow)(Display* dpy, GLXWindow win);
  GLXPixmap (CALL_OPENGL *glXCreatePixmap)(Display* dpy, GLXFBConfig config, Pixmap pixmap, const int* attrib_list);
  void (CALL_OPENGL *glXDestroyPixmap)(Display* dpy, GLXPixmap pixmap);
  GLXPbuffer (CALL_OPENGL *glXCreatePbuffer)(Display* dpy, GLXFBConfig config, const int* attrib_list);
  void (CALL_OPENGL *glXDestroyPbuffer)(Display* dpy, GLXPbuffer pbuf);
  void (CALL_OPENGL *glXQueryDrawable)(Display* dpy, GLXDrawable draw, int attribute, unsigned int* value);
  GLXContext (CALL_OPENGL *glXCreateNewContext)(Display* dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
  Bool (CALL_OPENGL *glXMakeContextCurrent)(Display* display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
  GLXDrawable (CALL_OPENGL *glXGetCurrentReadDrawable)(void);
  Display* (CALL_OPENGL *glXGetCurrentDisplay)(void);
  int (CALL_OPENGL *glXQueryContext)(Display* dpy, GLXContext ctx, int attribute, int* value);
  void (CALL_OPENGL *glXSelectEvent)(Display* dpy, GLXDrawable draw, unsigned long event_mask);
  void (CALL_OPENGL *glXGetSelectedEvent)(Display* dpy, GLXDrawable draw, unsigned long* event_mask);

//     {MESSAGE("XGetVisualInfo"), Cast::pointer<OpenGLFunction*>((void*)&XGetVisualInfo)},
//     {MESSAGE("XCreateWindow"), Cast::pointer<OpenGLFunction*>((void*)&XCreateWindow)},
//     {MESSAGE("XSync"), Cast::pointer<OpenGLFunction*>((void*)&XSync)},
  
  static Descriptor OPEN_GL_FUNCTIONS[] = {
    // GLX 1.0
    {MESSAGE("glXChooseVisual"), Cast::pointer<OpenGLFunction*>((void*)&glXChooseVisual)},
    {MESSAGE("glXCopyContext"), Cast::pointer<OpenGLFunction*>((void*)&glXCopyContext)},
    {MESSAGE("glXCreateContext"), Cast::pointer<OpenGLFunction*>((void*)&glXCreateContext)},
    {MESSAGE("glXCreateGLXPixmap"), Cast::pointer<OpenGLFunction*>((void*)&glXCreateGLXPixmap)},
    {MESSAGE("glXDestroyContext"), Cast::pointer<OpenGLFunction*>((void*)&glXDestroyContext)},
    {MESSAGE("glXDestroyGLXPixmap"), Cast::pointer<OpenGLFunction*>((void*)&glXDestroyGLXPixmap)},
    {MESSAGE("glXGetConfig"), Cast::pointer<OpenGLFunction*>((void*)&glXGetConfig)},
    {MESSAGE("glXGetCurrentContext"), Cast::pointer<OpenGLFunction*>((void*)&glXGetCurrentContext)},
    {MESSAGE("glXGetCurrentDrawable"), Cast::pointer<OpenGLFunction*>((void*)&glXGetCurrentDrawable)},
    {MESSAGE("glXIsDirect"), Cast::pointer<OpenGLFunction*>((void*)&glXIsDirect)},
    {MESSAGE("glXMakeCurrent"), Cast::pointer<OpenGLFunction*>((void*)&glXMakeCurrent)},
    {MESSAGE("glXQueryExtension"), Cast::pointer<OpenGLFunction*>((void*)&glXQueryExtension)},
    {MESSAGE("glXQueryVersion"), Cast::pointer<OpenGLFunction*>((void*)&glXQueryVersion)},
    {MESSAGE("glXSwapBuffers"), Cast::pointer<OpenGLFunction*>((void*)&glXSwapBuffers)},
    {MESSAGE("glXUseXFont"), Cast::pointer<OpenGLFunction*>((void*)&glXUseXFont)},
    {MESSAGE("glXWaitGL"), Cast::pointer<OpenGLFunction*>((void*)&glXWaitGL)},
    {MESSAGE("glXWaitX"), Cast::pointer<OpenGLFunction*>((void*)&glXWaitX)},
    
    // GLX 1.1
    MESSAGE("glXQueryExtensionsString"), Cast::pointer<OpenGLFunction*>((void*)&glXQueryExtensionsString),
    MESSAGE("glXGetClientString"), Cast::pointer<OpenGLFunction*>((void*)&glXGetClientString),
    MESSAGE("glXQueryServerString"), Cast::pointer<OpenGLFunction*>((void*)&glXQueryServerString),

    // GLX 1.2
    // TAG: ?
    
    // GLX 1.3
    MESSAGE("glXGetFBConfigs"), Cast::pointer<OpenGLFunction*>((void*)&glXGetFBConfigs),
    MESSAGE("glXChooseFBConfig"), Cast::pointer<OpenGLFunction*>((void*)&glXChooseFBConfig),
    MESSAGE("glXGetFBConfigAttrib"), Cast::pointer<OpenGLFunction*>((void*)&glXGetFBConfigAttrib),
    MESSAGE("glXGetVisualFromFBConfig"), Cast::pointer<OpenGLFunction*>((void*)&glXGetVisualFromFBConfig),
    MESSAGE("glXCreateWindow"), Cast::pointer<OpenGLFunction*>((void*)&glXCreateWindow),
    MESSAGE("glXDestroyWindow"), Cast::pointer<OpenGLFunction*>((void*)&glXDestroyWindow),
    MESSAGE("glXCreatePixmap"), Cast::pointer<OpenGLFunction*>((void*)&glXCreatePixmap),
    MESSAGE("glXDestroyPixmap"), Cast::pointer<OpenGLFunction*>((void*)&glXDestroyPixmap),
    MESSAGE("glXCreatePbuffer"), Cast::pointer<OpenGLFunction*>((void*)&glXCreatePbuffer),
    MESSAGE("glXDestroyPbuffer"), Cast::pointer<OpenGLFunction*>((void*)&glXDestroyPbuffer),
    MESSAGE("glXQueryDrawable"), Cast::pointer<OpenGLFunction*>((void*)&glXQueryDrawable),
    MESSAGE("glXCreateNewContext"), Cast::pointer<OpenGLFunction*>((void*)&glXCreateNewContext),
    MESSAGE("glXMakeContextCurrent"), Cast::pointer<OpenGLFunction*>((void*)&glXMakeContextCurrent),
    MESSAGE("glXGetCurrentReadDrawable"), Cast::pointer<OpenGLFunction*>((void*)&glXGetCurrentReadDrawable),
    MESSAGE("glXGetCurrentDisplay"), Cast::pointer<OpenGLFunction*>((void*)&glXGetCurrentDisplay),
    MESSAGE("glXQueryContext"), Cast::pointer<OpenGLFunction*>((void*)&glXQueryContext),
    MESSAGE("glXSelectEvent"), Cast::pointer<OpenGLFunction*>((void*)&glXSelectEvent),
    MESSAGE("glXGetSelectedEvent"), Cast::pointer<OpenGLFunction*>((void*)&glXGetSelectedEvent) 
  };
#endif
  
  /*
    GCC: referencing a member of a forward declared class results in "does not have member" but should have been something like "class not defined"
  */
  
}; // end of openGLContextImpl namespace

void OpenGLContext::onDestruction() throw() {
  deselect();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  if (renderingContext) {
    openGLContextImpl::wglDeleteContext((HGLRC)renderingContext);
    renderingContext = 0;
  }
  if (graphicsContextHandle) {
    ::DeleteDC((HDC)graphicsContextHandle);
    graphicsContextHandle = 0;
  }
  if (windowHandle) {
    ::DestroyWindow((HWND)windowHandle);
    windowHandle = 0;
  }
#else // unix
  if (renderingContext) {
    openGLContextImpl::glXDestroyContext((Display*)displayHandle, (GLXContext)renderingContext);
    renderingContext = 0;
  }
  if (windowHandle) {
    // TAG: do not destroy window in WindowImpl
    // is GLXwindow compatible with ::Window
    // GLX 1.3 openGLContextImpl::glXDestroyWindow((Display*)displayHandle, (GLXWindow)windowHandle);
    if (windowHandle) {
      ::XDestroyWindow((Display*)displayHandle, (::Window)windowHandle);
    }
  }
#endif
}

Empty OpenGLContext::initialize() throw(UserInterfaceException) {
  static SpinLock spinLock;
  spinLock.exclusiveLock();
  if (!openGLContextImpl::dynamicLinker) {
    // never release
    openGLContextImpl::dynamicLinker = new DynamicLinker(MESSAGE(_DK_SDU_MIP__BASE__OPENGL_LIBRARY));
    if (!openGLContextImpl::dynamicLinker) {
      spinLock.releaseLock();
      throw UserInterfaceException("Unable to load OpenGL module", this);
    }
    for (unsigned int i = 0; i < getArraySize(openGLContextImpl::OPEN_GL_FUNCTIONS); ++i) {
      *openGLContextImpl::OPEN_GL_FUNCTIONS[i].function =
        (openGLContextImpl::OpenGLFunction)openGLContextImpl::dynamicLinker->getUncertainSymbol(
          openGLContextImpl::OPEN_GL_FUNCTIONS[i].symbol
        ); // TAG: fix cast
      if (!*openGLContextImpl::OPEN_GL_FUNCTIONS[i].function) {
        spinLock.releaseLock();
        throw UserInterfaceException("Unable to load OpenGL module", this);
      }
    }
  }
  spinLock.releaseLock();
  
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
  
  windowHandle = ::CreateWindowEx(
    extendedStyle, // extended style
    "mip.sdu.dk/~fonseca/base/ui/Window", // class name
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
  assert(windowHandle, UserInterfaceException("Unable to create window", this));
  
  if (!(graphicsContextHandle = ::GetDC((HWND)windowHandle))) {
    ::DestroyWindow((HWND)windowHandle);
    windowHandle = 0;
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
  if (!(renderingContext = openGLContextImpl::wglCreateContext((HDC)graphicsContextHandle))) {
    ::DestroyWindow((HWND)windowHandle);
    throw UserInterfaceException("Unable to create rendering context", this);
  }
  if (!openGLContextImpl::wglMakeCurrent((HDC)graphicsContextHandle, (HGLRC)renderingContext)) {
    ::DestroyWindow((HWND)windowHandle);
    throw UserInterfaceException("Invalid rendering context", this);
  }
#else // unix
  {
    int errorBase;
    int eventBase;
    Bool result = openGLContextImpl::glXQueryExtension((Display*)displayHandle, &errorBase, &eventBase);
    assert(result == True, OpenGLException("OpenGL not supported", this));
    
    int major;
    int minor;
    result = openGLContextImpl::glXQueryVersion((Display*)displayHandle, &major, &minor);
    version = minimum(major, 255) << 16 | minimum(minor, 255) << 8;
  }
  
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  assert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
//   {
//     // GLX 1.3
//     int numberOfConfigs;
//     GLXFBConfig* configs = openGLContextImpl::glXGetFBConfigs((Display*)displayHandle, screenId, &numberOfConfigs);

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
//         int result = openGLContextImpl::glXGetFBConfigAttrib((Display*)displayHandle, configs[i], ATTRIBUTES[j], &value);
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
    
    visualInfo = openGLContextImpl::glXChooseVisual(
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
      int result = openGLContextImpl::glXGetConfig(
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
  // GLXWindow gLXWindow = openGLContextImpl::glXCreateWindow((Display*)displayHandle, GLXFBConfig config, ::Window win, 0);
  
  windowHandle = (void*)::XCreateSimpleWindow(
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
  
  renderingContext = openGLContextImpl::glXCreateContext(
    (Display*)displayHandle,
    visualInfo,
    0,
    (flags & OpenGLContext::DIRECT) ? True : False
  );
  ::XFree(visualInfo);
  if (renderingContext == 0) {
    ::XDestroyWindow((Display*)displayHandle, (::Window)windowHandle);
    windowHandle = 0;
    screenHandle = 0;
    throw OpenGLException("Unable to create rendering context", this);
  }
  makeCurrent();
  
  ::XSelectInput(
    (Display*)displayHandle,
    (::Window)windowHandle,
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
  return Empty();
}

OpenGLContext::OpenGLContext(const Position& position, const Dimension& dimension, unsigned int flags) throw(OpenGLException, UserInterfaceException)
  : WindowImpl(position, dimension, flags),
    prefixInitialization(initialize()),
    version(0),
    openGL() {
  onConstruction();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::InvalidateRect((HWND)windowHandle, 0, FALSE);
  ::UpdateWindow((HWND)windowHandle); // send WM_PAINT message
#else // unix
#endif // flavor
}

String OpenGLContext::getGLClientVendor() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String();
#else // unix
  return openGLContextImpl::glXGetClientString((Display*)displayHandle, GLX_VENDOR);
#endif // flavor
}

String OpenGLContext::getGLClientRelease() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String();
#else // unix
  return openGLContextImpl::glXGetClientString((Display*)displayHandle, GLX_VERSION);
#endif // flavor
}

String OpenGLContext::getGLClientExtensions() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String();
#else // unix
  return openGLContextImpl::glXGetClientString((Display*)displayHandle, GLX_EXTENSIONS);
#endif // flavor
}

String OpenGLContext::getGLServerVendor() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String();
#else // unix
  return openGLContextImpl::glXQueryServerString(
    (Display*)displayHandle,
    ::XScreenNumberOfScreen((Screen*)screenHandle),
    GLX_VENDOR
  );
#endif // flavor
}

String OpenGLContext::getGLServerRelease() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String();
#else // unix
  return openGLContextImpl::glXQueryServerString(
    (Display*)displayHandle,
    ::XScreenNumberOfScreen((Screen*)screenHandle),
    GLX_VERSION
  );
#endif // flavor
}

String OpenGLContext::getGLServerExtensions() const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String();
#else // unix
  return openGLContextImpl::glXQueryServerString(
    (Display*)displayHandle,
    ::XScreenNumberOfScreen((Screen*)screenHandle),
    GLX_EXTENSIONS
  );
#endif // flavor
}

bool OpenGLContext::isDirect() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return true;
#else // unix
  return openGLContextImpl::glXIsDirect(
    (Display*)displayHandle,
    (GLXContext)renderingContext
  ) == True;
#endif // flavor
}

bool OpenGLContext::hasCurrent() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return false; // TAG: fixme
#else // unix
  return openGLContextImpl::glXGetCurrentContext() != 0;
#endif // flavor
}

bool OpenGLContext::isCurrent() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return false; // TAG: fixme
#else // unix
  return openGLContextImpl::glXGetCurrentContext() == (GLXContext)renderingContext;
#endif // flavor
}

void OpenGLContext::makeCurrent() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  openGLContextImpl::wglMakeCurrent((HDC)graphicsContextHandle, (HGLRC)renderingContext);
#else // unix
  assert(
    openGLContextImpl::glXMakeCurrent(
      (Display*)displayHandle,
      (GLXDrawable)windowHandle,
      (GLXContext)renderingContext
    ) == True,
    OpenGLException(this)
  );
//   Bool result = openGLContextImpl::glXMakeContextCurrent(
//     (Display*)displayHandle,
//     (GLXDrawable)windowHandle,
//     (GLXDrawable)windowHandle,
//     (GLXContext)renderingContext
//   );
#endif // flavor
}

void OpenGLContext::deselect() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  openGLContextImpl::wglMakeCurrent(0, 0);
#else // unix
  assert(
    openGLContextImpl::glXMakeCurrent(
      (Display*)displayHandle,
      (GLXDrawable)None,
      (GLXContext)0
    ) == True,
    OpenGLException(this)
  );
//   Bool result = openGLContextImpl::glXMakeContextCurrent(
//     (Display*)displayHandle,
//     None,
//     None,
//     0
//   );
#endif // flavor
}

void OpenGLContext::swap() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetLastError(0); // TAG: remove when debugged
  if (!::SwapBuffers((HDC)graphicsContextHandle)) {
    fout << "Unable to swap buffers: " << ::GetLastError() << " " << graphicsContextHandle << " " << windowHandle << ENDL;
  }
//   BUG: ERROR_PROC_NOT_FOUND or ERROR_INVALID_WINDOW_HANDLE occur approx. 5% of the times
//   assert(
//     ::SwapBuffers((HDC)graphicsContextHandle),
//     OpenGLException("Unable to swap buffer", this)
//   );
#else // unix  
  openGLContextImpl::glXSwapBuffers((Display*)displayHandle, (GLXDrawable)windowHandle);
#endif // flavor
}

void OpenGLContext::swap(int plane) throw(OutOfRange, OpenGLException) {
  assert(
    (plane >= -numberOfUnderlayPlanes) && (plane <= numberOfOverlayPlanes),
    OutOfRange(this)
  );
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  static const UINT NATIVE_PLANES[15 - (-15) + 1] = {
    WGL_SWAP_UNDERLAY15,
    WGL_SWAP_UNDERLAY14,
    WGL_SWAP_UNDERLAY13,
    WGL_SWAP_UNDERLAY12,
    WGL_SWAP_UNDERLAY11,
    WGL_SWAP_UNDERLAY10,
    WGL_SWAP_UNDERLAY9,
    WGL_SWAP_UNDERLAY8,
    WGL_SWAP_UNDERLAY7,
    WGL_SWAP_UNDERLAY6,
    WGL_SWAP_UNDERLAY5,
    WGL_SWAP_UNDERLAY4,
    WGL_SWAP_UNDERLAY3,
    WGL_SWAP_UNDERLAY2,
    WGL_SWAP_UNDERLAY1,
    WGL_SWAP_MAIN_PLANE,
    WGL_SWAP_OVERLAY1,
    WGL_SWAP_OVERLAY2,
    WGL_SWAP_OVERLAY3,
    WGL_SWAP_OVERLAY4,
    WGL_SWAP_OVERLAY5,
    WGL_SWAP_OVERLAY6,
    WGL_SWAP_OVERLAY7,
    WGL_SWAP_OVERLAY8,
    WGL_SWAP_OVERLAY9,
    WGL_SWAP_OVERLAY10,
    WGL_SWAP_OVERLAY11,
    WGL_SWAP_OVERLAY12,
    WGL_SWAP_OVERLAY13,
    WGL_SWAP_OVERLAY14,
    WGL_SWAP_OVERLAY15
  };
  assert(
    openGLContextImpl::wglSwapLayerBuffers((HDC)graphicsContextHandle, NATIVE_PLANES[plane + 15]),
    OpenGLException("Unable to swap plane", this)
  );
#else // unix
  // TAG: fixme
#endif // flavor
}

void OpenGLContext::onDisplay() throw() {
  openGL.glClearColor(0.0, 0.0, 0.0, 1.0);
  openGL.glClear(OpenGL::COLOR_BUFFER_BIT);
  openGL.glFlush();
  swap();
}

void OpenGLContext::onResize(const Dimension& dimension) throw() {
  openGL.glViewport(0, 0, dimension.getWidth(), dimension.getHeight());
  // update(); // just request update
  onDisplay();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
