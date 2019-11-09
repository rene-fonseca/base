/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/PopUpWindow.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#else // unix (X11)
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void PopUpWindow::destroy() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)  
  if (graphicsContextHandle) {
    ::DeleteDC((HDC)graphicsContextHandle);
  }
  if (drawableHandle) {
    // nothing to destroy
  }
  BASSERT(screenHandle == 0);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
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

PopUpWindow::PopUpWindow(unsigned int flags) throw(UserInterfaceException)
  : GraphicsContext() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  drawableHandle = ::CreateWindowEx(
    WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, // extended style
    _COM_AZURE_DEV__BASE__ID_PREFIX L"/ui/WindowImpl", // class name
    L"", // window title
    WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // window style
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
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#else // unix
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  bassert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
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
  graphicsContextHandle = (void*)::XCreateGC((Display*)displayHandle, (::Window)drawableHandle, 0, 0);
  ::XSetForeground((Display*)displayHandle, (GC)graphicsContextHandle, whitePixel);
#endif // flavor
  construct();
  invalidate();
  update();
}

PopUpWindow::PopUpWindow(const Position& position, const Dimension& dimension, unsigned int flags) throw(UserInterfaceException)
  : GraphicsContext(position, dimension, flags) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  RECT rect;
  rect.left = position.getX();
  rect.right = position.getX() + dimension.getWidth();
  rect.top = position.getY();
  rect.bottom = position.getY() + dimension.getHeight();
  
  DWORD extendedStyle = WS_EX_NOACTIVATE | WS_EX_TOPMOST;
  DWORD style = WS_POPUP;
  ::AdjustWindowRectEx(&rect, style, FALSE, extendedStyle);
  
  drawableHandle = ::CreateWindowEx(
    extendedStyle, // extended style
    _COM_AZURE_DEV__BASE__ID_PREFIX L"/ui/WindowImpl", // class name
    L"", // window title
    style | // window style
    WS_DISABLED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
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
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#else // unix
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  bassert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
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
  graphicsContextHandle = (void*)::XCreateGC((Display*)displayHandle, (::Window)drawableHandle, 0, 0);
  ::XSetForeground((Display*)displayHandle, (GC)graphicsContextHandle, whitePixel);
#endif // flavor
  construct();
  invalidate();
  update();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
