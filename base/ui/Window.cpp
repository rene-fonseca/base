/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#  include <X11/cursorfont.h>
#  include <X11/keysym.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void Window::onDestruction() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  if (graphicsContextHandle) {
    ::DeleteDC((HDC)graphicsContextHandle);
    graphicsContextHandle = 0;
  }
  if (windowHandle) {
    ::DestroyWindow((HWND)windowHandle); // TAG: recursive?
    windowHandle = 0;
  }
#else // unix
  if (graphicsContextHandle) {
    graphicsContextHandle = 0; // nothing to destroy
  }
  if (windowHandle) {
    ::XDestroyWindow((Display*)displayHandle, (::Window)windowHandle);
    windowHandle = 0;
  }
  if (screenHandle) {
    screenHandle = 0; // nothing to destroy
  }
#endif
}

Window::Window(const Position& position, const Dimension& dimension, unsigned int flags) throw(UserInterfaceException)
  : WindowImpl(position, dimension, flags) {
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
  //   enum ViewState { // TAG: other name?
  //     MINIMIZED,
  //     MAXIMIZED,
  //     VISIBLE
  //   };
  
  windowHandle = ::CreateWindowEx(
    extendedStyle, // extended style
    "mip.sdu.dk/~fonseca/base/ui/Window", // class name
    "", // window title
    style | // window style
    WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // required for OpenGL // TAG: what should this be
    rect.left, // x position,
    rect.top, // y position,
    rect.right - rect.left,
    rect.bottom - rect.top,
    0, // no parent window
    0, // use the window class menu
    (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
    0 // application window data structure
  );
  if (!(HWND)windowHandle) {
    throw UserInterfaceException("Unable to create window", this);
  }
  
  if (!(graphicsContextHandle = ::GetDC((HWND)windowHandle))) {
    ::DestroyWindow((HWND)windowHandle);
    windowHandle = 0;
    throw UserInterfaceException("Unable to connect to device context", this);
  }
  
  onConstruction();
  
  ::InvalidateRect((HWND)windowHandle, 0, FALSE);
  ::UpdateWindow((HWND)windowHandle); // send WM_PAINT message
#else // unix
  fout << MESSAGE("Number of available screens: ") << ::XScreenCount((Display*)displayHandle) << ENDL;
  fout << MESSAGE("Default screen: ") << ::XDefaultScreen((Display*)displayHandle) << ENDL;
  
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  assert(screenHandle, UserInterfaceException("Unable to open screen", this));

  fout << MESSAGE("default depth of screen: ") << DefaultDepthOfScreen((Screen*)screenHandle) << ENDL;
  
  int blackPixel = ::XBlackPixelOfScreen((Screen*)screenHandle);
  int whitePixel = ::XWhitePixelOfScreen((Screen*)screenHandle);
  
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
  // TAG: detect error and raise exception
  
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
  graphicsContextHandle = (void*)::XCreateGC((Display*)displayHandle, (::Window)windowHandle, 0, 0);
  ::XSetForeground((Display*)displayHandle, (GC)graphicsContextHandle, whitePixel);

  setTitle(title);
  setIconTitle(title);
#endif // flavor
}

// TAG: belongs in graphics context class
void Window::clear() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  //::GetClientRect((HWND)windowHandle, &rect);
  rect.left = 0;
  rect.top = 0;
  rect.right = dimension.getWidth(); // do not subtract 1
  rect.bottom = dimension.getHeight(); // do not subtract 1
  assert(
    ::FillRect(
      (HDC)graphicsContextHandle,
      &rect,
      (HBRUSH)::GetStockObject(WHITE_BRUSH) // TAG: fixme
    ),
    UserInterfaceException(this)
  );
#else // unix
  ::XClearWindow(
    (Display*)displayHandle,
    (::Window)windowHandle
  );
#endif // flavor
}

void Window::clear(const Position& position, const Dimension& dimension) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = position.getX();
  rect.top = position.getY();
  rect.right = rect.left + dimension.getWidth(); // do not subtract 1
  rect.bottom = rect.bottom + dimension.getHeight(); // do not subtract 1
  assert(
    ::FillRect(
      (HDC)graphicsContextHandle,
      &rect,
      (HBRUSH)::GetStockObject(WHITE_BRUSH) // TAG: fixme
    ),
    UserInterfaceException(this)
  );
#else // unix
  ::XClearArea(
    (Display*)displayHandle,
    (::Window)windowHandle,
    position.getX(),
    position.getY(),
    dimension.getWidth(),
    dimension.getHeight(),
    False
  );
#endif // flavor
}

void Window::point(const Position& position, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   ::MoveToEx(
//     (HDC)graphicsContextHandle,
//     a.getX(),
//     a.getY(),
//     0
//   );
#else // unix
  ::XDrawPoint(
    (Display*)displayHandle,
    (::Window)windowHandle,
    (GC)graphicsContextHandle,
    position.getX(),
    position.getY()
  );
#endif // flavor
}

// void Window::points(const Array<Position>& position, unsigned int flags) throw(UserInterfaceException) {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
// #else // unix
//   XPoint* points = 0;
//   ::XDrawPoints(
//     (Display*)displayHandle,
//     (::Window)windowHandle,
//     (GC)graphicsContextHandle,
//     points,
//     numberOfPoints,
//     CoordModeOrigin // mode
//   );
// #endif // flavor  
// }

void Window::line(const Position& a, const Position& b, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::MoveToEx(
    (HDC)graphicsContextHandle,
    a.getX(),
    a.getY(),
    0
  );
  assert(
    ::LineTo(
      (HDC)graphicsContextHandle,
      b.getX(),
      b.getY()
    ),
    UserInterfaceException(this)
  );
#else // unix
  ::XDrawLine(
    (Display*)displayHandle,
    (::Window)windowHandle,
    (GC)graphicsContextHandle,
    a.getX(),
    a.getY(),
    b.getX(),
    b.getY()
  );
#endif // flavor  
}

// void Window::polyline(const Array<Position>& points, unsigned int flags) throw(UserInterfaceException) {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   ASSERT(sizeof(POINTS) == (2 * sizeof(int)));
//   ::Polyline(
//     (HDC)graphicsContextHandle,
//     points.getElements(),
//     points.getSize()
//   );
// #else // unix
// //   ::XDrawLine(
// //     (Display*)displayHandle,
// //     (::Window)windowHandle,
// //     (GC)graphicsContextHandle,
// //     a.getX(),
// //     a.getY(),
// //     b.getX(),
// //     b.getY()
// //   );
// #endif // flavor  
// }

void Window::arc(const Position& position, const Dimension& dimension, int start, int stop, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   BOOL ::Arc(
//     (HDC)graphicsContextHandle, // handle to device context
//     int nLeftRect,   // x-coord of rectangle's upper-left corner
//     int nTopRect,    // y-coord of rectangle's upper-left corner
//     int nRightRect,  // x-coord of rectangle's lower-right corner
//     int nBottomRect, // y-coord of rectangle's lower-right corner
//     int nXStartArc,  // x-coord of first radial ending point
//     int nYStartArc,  // y-coord of first radial ending point
//     int nXEndArc,    // x-coord of second radial ending point
//     int nYEndArc     // y-coord of second radial ending point
//   );
#else // unix
  if (flags & Window::FILLED) {
    ::XFillArc(
      (Display*)displayHandle,
      (::Window)windowHandle,
      (GC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight(),
      start,
      stop
    );
  } else {
    ::XDrawArc(
      (Display*)displayHandle,
      (::Window)windowHandle,
      (GC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight(),
      start,
      stop
    );
  }
#endif // flavor  
}

void Window::rectangle(const Position& position, const Dimension& dimension, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Rectangle(
      (HDC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      position.getX() + dimension.getWidth() - 1,
      position.getY() + dimension.getHeight() - 1
    ),
    UserInterfaceException(this)
  );
#else // unix
  if (flags & Window::FILLED) {
    ::XFillRectangle(
      (Display*)displayHandle,
      (::Window)windowHandle,
      (GC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight()
    );
  } else {
    ::XDrawRectangle(
      (Display*)displayHandle,
      (::Window)windowHandle,
      (GC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight()
    );
  }
#endif // flavor  
}
#if 0
::XFillArcs(
  (Display*)displayHandle,
  (::Window)windowHandle,
  (GC)graphicsContextHandle,
  arcs, // XArc
  narcs // int
);

::XFillPolygon(
  (Display*)displayHandle,
  (::Window)windowHandle,
  (GC)graphicsContextHandle,
  points,
  npoints,
  shape,
  mode
);

void Window::rectangle(const Array<Region>& rectangles, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  ::XFillRectangles(
    (Display*)displayHandle,
    (::Window)windowHandle,
    (GC)graphicsContextHandle,
    rectangles,
    rectangles.getLength()
  );
#endif // flavor  
}
#endif

Dimension Window::getDimensionOf(const String& text) const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  SIZE size;
  assert(
    ::GetTextExtentPoint(
      (HDC)graphicsContextHandle,
      text.getElements(),
      text.getLength(),
      &size
    ),
    UserInterfaceException(this)
  );
  return Dimension(size.cx, size.cy);
#else // unix
  return Dimension();
#endif // flavor
}

// TAG: whole characters fitting in an area
// BOOL GetTextExtentExPoint(
//   HDC hdc,         // handle to DC
//   LPCTSTR lpszStr, // character string
//   int cchString,   // number of characters
//   int nMaxExtent,  // maximum width of formatted string
//   LPINT lpnFit,    // maximum number of characters
//   LPINT alpDx,     // array of partial string widths
//   LPSIZE lpSize    // string dimensions
// );

// TAG: need ucs2 method
void Window::text(const Position& position, const Dimension& dimension, const String& text, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = position.getX();
  rect.top = position.getY();
  rect.right = rect.left + dimension.getWidth() - 1;
  rect.bottom = rect.top + dimension.getHeight() - 1;
  assert(
    ::DrawText(
      (HDC)graphicsContextHandle,
      text.getElements(),
      text.getLength(),
      &rect,
      DT_LEFT|DT_TOP // options // TAG: fixme
    ),
    UserInterfaceException(this)
  );
#else // unix
	::XDrawString(
    (Display*)displayHandle,
    (::Window)windowHandle,
    (GC)graphicsContextHandle,
    position.getX(),
    position.getY(),
    text.getElements(),
    text.getLength()
  );
#endif // flavor  
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
