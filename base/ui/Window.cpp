/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Window.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void Window::destroy() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  if (graphicsContextHandle) {
    ::DeleteDC((HDC)graphicsContextHandle);
  }
  if (drawableHandle) {
    // nothing to destroy
  }
  ASSERT(screenHandle == 0);
#else // unix
  if (graphicsContextHandle) {
    // nothing to destroy
  }
  if (drawableHandle) {
    ::XDestroyWindow((Display*)displayHandle, (::Window)drawableHandle);
  }
  if (screenHandle) {
    // nothing to destroy
  }
#endif
  WindowImpl::destroy();
}

Window::Window(unsigned int flags) throw(UserInterfaceException)
  : GraphicsContext() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  drawableHandle = ::CreateWindowEx(
    WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, // extended style
    "http://mip.sdu.dk/~fonseca/base/ui/WindowImpl", // class name
    "", // window title
    WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // window style - required for OpenGL // TAG: what should this be
    CW_USEDEFAULT, // x position
    0, // y position (ignored)
    CW_USEDEFAULT, // width
    0, // height (ignored)
    0, // no parent window
    0, // use the window class menu
    (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
    0 // application window data structure
  );
  if (!(HWND)drawableHandle) {
    throw UserInterfaceException("Unable to create window", this);
  }
  
  if (!(graphicsContextHandle = ::GetDC((HWND)drawableHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw UserInterfaceException("Unable to connect to device context", this);
  }
#else // unix
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  assert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
  int blackPixel = ::XBlackPixelOfScreen((Screen*)screenHandle);
  int whitePixel = ::XWhitePixelOfScreen((Screen*)screenHandle);
  
  drawableHandle = (void*)::XCreateSimpleWindow(
    (Display*)displayHandle,
    DefaultRootWindow((Display*)displayHandle),
    1, // x
    1, // y
    0, // width
    0, // height
    0, // border width
    blackPixel, // border color
    blackPixel // background
  );
  ::XFlush((Display*)displayHandle);
  // TAG: detect error and raise exception
  
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
  graphicsContextHandle = (void*)::XCreateGC(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    0,
    0
  );
  ::XSetForeground(
    (Display*)displayHandle,
    (GC)graphicsContextHandle,
    whitePixel
  );
#endif // flavor
  construct();
  invalidate();
  update();
}

Window::Window(
  const Position& position,
  const Dimension& dimension,
  unsigned int flags) throw(UserInterfaceException)
  : GraphicsContext(position, dimension, flags) {
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
  
  // WS_EX_TOPMOST, WS_EX_TRANSPARENT, WS_EX_LAYERED
  // normal styles: WS_MAXIMIZE WS_MINIMIZE WS_VISIBLE
  
  drawableHandle = ::CreateWindowEx(
    extendedStyle, // extended style
    "http://mip.sdu.dk/~fonseca/base/ui/WindowImpl", // class name
    "", // window title
    style | // window style
    WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // required for OpenGL // TAG: what should this be
    rect.left, // x position
    rect.top, // y position
    rect.right - rect.left,
    rect.bottom - rect.top,
    0, // no parent window
    0, // use the window class menu
    (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
    0 // application window data structure
  );
  if (!(HWND)drawableHandle) {
    throw UserInterfaceException("Unable to create window", this);
  }
  
  if (!(graphicsContextHandle = ::GetDC((HWND)drawableHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw UserInterfaceException("Unable to connect to device context", this);
  }
#else // unix
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  assert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
  int blackPixel = ::XBlackPixelOfScreen((Screen*)screenHandle);
  int whitePixel = ::XWhitePixelOfScreen((Screen*)screenHandle);
  
  drawableHandle = (void*)::XCreateSimpleWindow(
    (Display*)displayHandle,
    DefaultRootWindow((Display*)displayHandle),
    position.getX(),
    position.getY(),
    maximum(dimension.getWidth(), 1U), // TAG: X-Win32 5.4 does not accept 0
    maximum(dimension.getHeight(), 1U), // TAG: X-Win32 5.4 does not accept 0
    0, // border width
    blackPixel, // border color
    blackPixel // background
  );
  // TAG: detect error and raise exception
 
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
  graphicsContextHandle = (void*)::XCreateGC(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    0,
    0
  );
  ::XSetForeground(
    (Display*)displayHandle,
    (GC)graphicsContextHandle,
    whitePixel
  );
#endif // flavor
  construct();
  invalidate();
  update();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
