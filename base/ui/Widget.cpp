/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Widget.h>

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

Widget::GraphicsContextObjectHandle::~GraphicsContextObjectHandle() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (handle) {
    ::DeleteObject((HGDIOBJ)handle);
  }
#else // unix
#endif // flavor
}

Color Widget::getNamedColor(NamedColor color) throw() {
  static uint32 NAMED_COLORS[] = { // keep in sync with Widget::NamedColors
    0xfffccc, // Almond(Blanched)
    0x00ffff, // Aqua
    0x60cc93, // Aquamarine
    0x6fffc3, // Aquamarine(Light)
    0x33cc99, // Aquamarine(Med)
    0xf6f6cc, // Beige
    0xfff3c3, // Bisque
    0x000000, // Black
    0x0000ff, // Blue
    0x6f9f9f, // Blue(Cadet)
    0x6f9f90, // Blue(Cadet3)
    0x33336f, // Blue(CornFlower)
    0x6396fc, // Blue(CornFlower-Light)
    0x00009c, // Blue(Dark)
    0x6c339f, // Blue(DarkSlate)
    0x99ccff, // Blue(Light)
    0x3333cc, // Blue(Med)
    0x3f33f3, // Blue(Midnight)
    0x33339f, // Blue(Navy)
    0x3c3cff, // Blue(Neon)
    0x3399cc, // Blue(Sky)
    0x336c9f, // Blue(Steel)
    0x993cf3, // Blue(Violet)
    0xc69633, // Brass
    0x9c6963, // Bronze
    0x963939, // Brown
    0x663033, // Brown(Dark)
    0xf6ccc0, // Brown(Faded)
    0x6fff00, // Chartruse
    0xc3690f, // Chocolate
    0x6c3306, // Chocolate(Bakers)
    0x6c3336, // Chocolate(SemiSweet)
    0xc96333, // Copper
    0x39666f, // Copper(DarkGreen)
    0x636f66, // Copper(Green)
    0xff6f60, // Coral
    0xff6f00, // Coral(Bright)
    0xfff9cc, // Cornsilk
    0xcc033c, // Crimson
    0x00ffff, // Cyan
    0x009c9c, // Cyan(Dark)
    0x9f3333, // Firebrick
    0xff00ff, // Fushia
    0xffcc00, // Gold
    0xc9c909, // Gold(Bright)
    0xc6c63c, // Gold(Old)
    0xcccc60, // Goldenrod
    0xc9960c, // Goldenrod(Dark)
    0xf9f99f, // Goldenrod(Med)
    0x909090, // Gray
    0x3f3f3f, // Gray(DarkSlate)
    0x666666, // Gray(Dark)
    0x999999, // Gray(Light)
    0xc0c0c0, // Gray(Lighter)
    0x009000, // Green
    0x006300, // Green(Dark)
    0x3f3f3f, // Green(Forest)
    0x9fcc9f, // Green(Pale)
    0x99cc33, // Green(Yellow)
    0x9f9f6f, // Khaki
    0xccc66c, // Khaki(Light)
    0x00ff00, // Lime
    0xff00ff, // Magenta
    0xcc009c, // Magenta(Dark)
    0x900000, // Maroon
    0x9f336c, // Maroon3
    0x000090, // Navy
    0x909000, // Olive
    0x666c3f, // Olive(Dark)
    0x3f3f3f, // Olive(Darker)
    0xff6f00, // Orange
    0xff9c00, // Orange(Light)
    0xcc60cc, // Orchid
    0x9933cc, // Orchid(Dark)
    0xf99cf9, // Pink
    0xcc9f9f, // Pink(Dusty)
    0x900090, // Purple
    0x960669, // Purple(Dark)
    0xc9c9f3, // Quartz
    0xff0000, // Red
    0x9c0000, // Red(Dark)
    0xff3300, // Red(Orange)
    0x966363, // Rose(Dusty)
    0x6f3333, // Salmon
    0x9c0606, // Scarlet
    0x9f6c33, // Sienna
    0xf6f9f9, // Silver
    0x39c0cf, // Sky(Summer)
    0xcc9360, // Tan
    0x96693f, // Tan(Dark)
    0x009090, // Teal
    0xc9cfc9, // Thistle
    0x9cf9f9, // Turquoise
    0x00cfc0, // Turquoise(Dark)
    0x9900ff, // Violet
    0x6600cc, // Violet(Blue)
    0xcc3399, // Violet(Red)
    0xffffff, // White
    0xf9fcc6, // White(Antique)
    0xcfc996, // Wood(Curly)
    0x966f33, // Wood(Dark)
    0xf9c396, // Wood(Light)
    0x969063, // Wood(Med)
    0xffff00 // Yellow
  };
  Color result;
  result.value = NAMED_COLORS[color];
  return result;
}

void Widget::onDestruction() throw() {
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

Widget::Widget(Window& owner) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  windowHandle = ::CreateWindowEx(
    0, // extended style
    "mip.sdu.dk/~fonseca/base/ui/Window", // class name // TAG: "mip.sdu.dk/~fonseca/base/ui/Widget"
    "", // window title
    WS_CHILD | WS_VISIBLE, // window style // TAG: what should this be
    position.getX(),
    position.getY(),
    dimension.getWidth(),
    dimension.getHeight(),
    (HWND)owner.windowHandle, // parent window
    0, // use the window class menu
    (HINSTANCE)::GetModuleHandle(0), // the instance that owns the window (ignored on NT)
    0 // application window data structure
  );
  if (!windowHandle) {
    throw UserInterfaceException("Unable to create widget", this);
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
  int screenId = ::XDefaultScreen((Display*)displayHandle);
  screenHandle = ::XScreenOfDisplay((Display*)displayHandle, screenId);
  assert(screenHandle, UserInterfaceException("Unable to open screen", this));
  
  int blackPixel = ::XBlackPixelOfScreen((Screen*)screenHandle);
  int whitePixel = ::XWhitePixelOfScreen((Screen*)screenHandle);
  
  windowHandle = (void*)::XCreateSimpleWindow(
    (Display*)displayHandle,
    (::Window)owner.windowHandle, // parent
    position.getX(),
    position.getY(),
    dimension.getWidth(),
    dimension.getHeight(),
    0, // border width
    blackPixel, // border color
    blackPixel // background
  );

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
  graphicsContextHandle = (void*)::XCreateGC(
    (Display*)displayHandle,
    (::Window)windowHandle,
    0,
    0
  );
  ::XSetForeground((Display*)displayHandle, (GC)graphicsContextHandle, whitePixel);
  
  onConstruction();
#endif // flavor
}

Position Widget::getBindingOffset(Binding binding) const throw() {
  const Position position = getPosition();
  const Dimension dimension = getDimension();
  int x = position.getX();
  int y = position.getY();
  switch (binding) {
  case UPPER_LEFT:
    break;
  case UPPER_CENTER:
    x += dimension.getWidth()/2;
    break;
  case UPPER_RIGHT:
    x += dimension.getWidth();
    break;
  case MIDDLE_LEFT:
    y += dimension.getHeight()/2;
    break;
  case MIDDLE_CENTER:
    x += dimension.getWidth()/2;
    y += dimension.getHeight()/2;
    break;
  case MIDDLE_RIGHT:
    x += dimension.getWidth();
    y += dimension.getHeight()/2;
    break;
  case LOWER_LEFT:
    y += dimension.getHeight();
    break;
  case LOWER_CENTER:
    x += dimension.getWidth()/2;
    y += dimension.getHeight();
    break;
  case LOWER_RIGHT:
    x += dimension.getWidth();
    y += dimension.getHeight();
  }
  return Position(x, y);
}

Widget::Pen::Pen(Style style, Color color, unsigned int width) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static const int NATIVE_STYLES[] = { // keep in sync with Widget::Style
    PS_NULL, // NONE
    PS_SOLID, // SOLID
    PS_DASH, // DASH
    PS_DOT, // DOT
    PS_DASHDOT, // DASH_DOT
    PS_DASHDOTDOT // DASH_DOT_DOT
  };
  if (width == 0) {
    style = NONE; // TAG: is this ok
  }
  HPEN pen = ::CreatePen(
    NATIVE_STYLES[style],
    width,
    color.value
  );
  assert(pen, UserInterfaceException(this));
  setHandle(pen);
#else // unix
#endif // flavor
}

Widget::Brush::Brush(SystemColor color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static const int NATIVE_COLORS[] = { // keep in sync with Widget::NamedColor
    COLOR_WINDOW, // WINDOW_BACKGROUND
    COLOR_WINDOWFRAME, // WINDOW_FRAME
    COLOR_WINDOWTEXT, // WINDOW_TEXT
    COLOR_MENU, // MENU
    COLOR_MENUTEXT, // MENU_TEXT
    COLOR_BTNTEXT, // BUTTON_TEXT
    COLOR_GRAYTEXT, // DISABLED_TEXT
    COLOR_HIGHLIGHTTEXT // HIGHLIGHTED_TEXT
  };
  HBRUSH brush = ::GetSysColorBrush(NATIVE_COLORS[color]);
  assert(brush, UserInterfaceException(this));
  setHandle(brush);
  // prevent destruction of object
  ReferenceCountedObjectPointerImpl(*handle).addReference();
  // TAG: need method to query to current number of references
#else // unix
#endif // flavor
}

Widget::Brush::Brush(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HBRUSH brush = ::CreateSolidBrush(color.value);
  assert(brush, UserInterfaceException(this));
  setHandle(brush);
#else // unix
#endif // flavor
}

Widget::Brush::Brush(unsigned int color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HBRUSH brush = ::CreateSolidBrush(color & 0xffffff);
  assert(brush, UserInterfaceException(this));
  setHandle(brush);
#else // unix
#endif // flavor
}

Widget::Font::Font(const String& name) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: fixme
#else // unix
//   fs = ::XCreateFontSet(
//     (Display*)displayHandle,
//     DEFAULT_FONT,
//     &missingList,
//     &missingCount,
//     &string
//   );
//   assert(fs, UserInterfaceException(this));
#endif // flavor
}

void Widget::setPen(Pen pen) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  this->pen = pen; // put lock on pen
  HGDIOBJ handle = ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)pen.getHandle()
  );
#else // unix
#endif // flavor
}

void Widget::setBrush(Brush brush) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  this->brush = brush; // put lock on brush
  HGDIOBJ handle = ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)brush.getHandle()
  );
#else // unix
#endif // flavor
}

void Widget::setFont(Font font) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HGDIOBJ handle = ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)font.getHandle()
  );
#else // unix
#endif // flavor  
}

void Widget::setBackgroundMode(bool transparent) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetBkMode(
    (HDC)graphicsContextHandle,
    transparent ? TRANSPARENT : OPAQUE
  );
#else // unix
#endif // flavor
};

void Widget::setBackgroundColor(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetBkColor(
    (HDC)graphicsContextHandle,
    color.value
  );
#else // unix
#endif // flavor
};

void Widget::setTextColor(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetTextColor(
    (HDC)graphicsContextHandle,
    color.value
  );
#else // unix
#endif // flavor
};

void Widget::setTextAlignment(unsigned int alignment) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  unsigned int nativeAlignment = 0;
  if (alignment & Alignment::LEFT) {
    nativeAlignment |= TA_LEFT;
  } else if (alignment & Alignment::CENTER) {
    nativeAlignment |= TA_CENTER;
  } else if (alignment & Alignment::RIGHT) {
    nativeAlignment |= TA_RIGHT;
  }
  if (alignment & Alignment::TOP) {
    nativeAlignment |= TA_TOP;
  } else if (alignment & Alignment::MIDDLE) {
    nativeAlignment |= VTA_CENTER;
  } else if (alignment & Alignment::BOTTOM) {
    nativeAlignment |= TA_BOTTOM;
  }
  ::SetTextAlign(
    (HDC)graphicsContextHandle,
    nativeAlignment
  );
#else // unix
#endif // flavor
}

// Dimension Widget::getDimension() const throw(UserInterfaceException) {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   RECT rect;
//   ::GetClientRect((HWND)windowHandle, &rect);
//   return Region();
// #else // unix
// #endif // flavor
// }

void Widget::clear() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  //::GetClientRect((HWND)windowHandle, &rect);
  rect.left = 0;
  rect.top = 0;
  rect.right = dimension.getWidth(); // do not subtract 1
  rect.bottom = dimension.getHeight(); // do not subtract 1
  // backgroud brush?
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

void Widget::clear(const Position& position, const Dimension& dimension) throw(UserInterfaceException) {
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

void Widget::point(const Position& position, unsigned int flags) throw(UserInterfaceException) {
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

// void Widget::points(const Array<Position>& position, unsigned int flags) throw(UserInterfaceException) {
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

void Widget::line(const Position& a, const Position& b, unsigned int flags) throw(UserInterfaceException) {
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

// void Widget::polyline(const Array<Position>& points, unsigned int flags) throw(UserInterfaceException) {
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

void Widget::arc(const Position& position, const Dimension& dimension, int start, int stop, unsigned int flags) throw(UserInterfaceException) {
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
  if (flags & Widget::FILLED) {
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

void Widget::rectangle(const Position& a, const Position& b, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Rectangle(
      (HDC)graphicsContextHandle,
      a.getX(),
      a.getY(),
      b.getX(),
      b.getY()
    ),
    UserInterfaceException(this)
  );
#else // unix
//   if (flags & Widget::FILLED) {
//     ::XFillRectangle(
//       (Display*)displayHandle,
//       (::Window)windowHandle,
//       (GC)graphicsContextHandle,
//       position.getX(),
//       position.getY(),
//       dimension.getWidth(),
//       dimension.getHeight()
//     );
//   } else {
//     ::XDrawRectangle(
//       (Display*)displayHandle,
//       (::Window)windowHandle,
//       (GC)graphicsContextHandle,
//       position.getX(),
//       position.getY(),
//       dimension.getWidth(),
//       dimension.getHeight()
//     );
//   }
#endif // flavor  
}

void Widget::rectangle(const Position& position, const Dimension& dimension, unsigned int flags) throw(UserInterfaceException) {
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
  if (flags & Widget::FILLED) {
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

void Widget::rectangle(const Position& a, const Position& b, Brush brush, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = a.getX();
  rect.top = a.getY();
  rect.right = b.getX();
  rect.bottom = b.getY();
  assert(
    ::FillRect(
      (HDC)graphicsContextHandle,
      &rect,
      (HBRUSH)brush.getHandle()
    ),
    UserInterfaceException(this)
  );
#else // unix
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

void Widget::rectangle(const Array<Region>& rectangles, unsigned int flags) throw(UserInterfaceException) {
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

void Widget::ellipse(const Position& a, const Position& b, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Ellipse(
      (HDC)graphicsContextHandle,
      a.getX(),
      a.getY(),
      b.getX(),
      b.getY()
    ),
    UserInterfaceException(this)
  );
#else // unix
#endif // flavor  
}

  /**
  BOOL Pie(
  HDC hdc,         // handle to DC
  int nLeftRect,   // x-coord of upper-left corner of rectangle
  int nTopRect,    // y-coord of upper-left corner of rectangle
  int nRightRect,  // x-coord of lower-right corner of rectangle
  int nBottomRect, // y-coord of lower-right corner of rectangle
  int nXRadial1,   // x-coord of first radial's endpoint
  int nYRadial1,   // y-coord of first radial's endpoint
  int nXRadial2,   // x-coord of second radial's endpoint
  int nYRadial2    // y-coord of second radial's endpoint
);
*/

Dimension Widget::getDimensionOf(const String& text) const throw(UserInterfaceException) {
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
void Widget::text(const Position& position, const Dimension& dimension, const String& text, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  UINT nativeTextFormat = 0;
  if (flags & TextFormat::LEFT) {
    nativeTextFormat |= DT_LEFT;
  } else if (flags & TextFormat::CENTER) {
    nativeTextFormat |= DT_CENTER;
  } else if (flags & TextFormat::RIGHT) {
    nativeTextFormat |= DT_RIGHT;
  }
  if (flags & TextFormat::TOP) {
    nativeTextFormat |= DT_TOP;
  } else if (flags & TextFormat::MIDDLE) {
    nativeTextFormat |= DT_VCENTER;
  } else if (flags & TextFormat::BOTTOM) {
    nativeTextFormat |= DT_BOTTOM;
  }
  nativeTextFormat |= (flags & TextFormat::MULTIPLE_LINES) ? 0 : DT_SINGLELINE;
  nativeTextFormat |= (flags & TextFormat::BREAK_WORDS) ? DT_WORDBREAK : 0;
  nativeTextFormat |= (flags & TextFormat::PREFIX) ? 0 : DT_NOPREFIX;

  RECT rect;
  rect.left = position.getX();
  rect.top = position.getY();
  rect.right = rect.left + dimension.getWidth() - 1;
  rect.bottom = rect.top + dimension.getHeight() - 1;
  assert(
    ::DrawTextEx(
      (HDC)graphicsContextHandle,
      (char*)text.getElements(), // not allowed to change string
      text.getLength(),
      &rect,
      nativeTextFormat,
      0
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
