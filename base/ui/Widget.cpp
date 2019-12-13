/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Widget.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#  include <base/platforms/os/unix/X11.h>
#if defined(_COM_AZURE_DEV__BASE__USE_X11)
#  include <X11/cursorfont.h>
#  include <X11/keysym.h>
#endif
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void Widget::destroy() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)  
  if (graphicsContextHandle) {
    ::DeleteDC((HDC)graphicsContextHandle);
  }
  if (drawableHandle) {
    // nothing to destroy
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
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

Widget::Widget(Window& owner) throw(UserInterfaceException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  drawableHandle = ::CreateWindowEx(
    0, // extended style
    _COM_AZURE_DEV__BASE__ID_PREFIX L"/ui/WindowImpl", // class name
    L"", // window title
    WS_CHILD | WS_VISIBLE, // window style // TAG: what should this be
    0, // x
    0, // y
    0, // width
    0, // height
    (HWND)owner.drawableHandle, // parent window
    0, // use the window class menu
    (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
    0 // application window data structure
  );
  if (!drawableHandle) {
    throw UserInterfaceException("Unable to create widget.", this);
  }
  if (!(graphicsContextHandle = ::GetDC((HWND)drawableHandle))) {
    ::DestroyWindow((HWND)drawableHandle);
    drawableHandle = 0;
    throw UserInterfaceException("Unable to connect to device context.", this);
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  bassert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
  int blackPixel = ::XBlackPixelOfScreen((Screen*)screenHandle);
  int whitePixel = ::XWhitePixelOfScreen((Screen*)screenHandle);

  drawableHandle = (void*)::XCreateSimpleWindow(
    (Display*)displayHandle,
    (::Window)owner.drawableHandle, // parent
    0, // x
    0, // y
    1, // width // TAG: X-Win32 5.4 does not accept 0
    1, // height // TAG: X-Win32 5.4 does not accept 0
    0, // border width
    blackPixel, // border color
    blackPixel // background
  );
  if (!drawableHandle) {
    throw UserInterfaceException("Unable to create widget.", this);
  }
  
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

Widget::~Widget() noexcept
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
