/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/GraphicsContext.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix (X11)
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

GraphicsContext::GraphicsContextObjectHandle::~GraphicsContextObjectHandle() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (graphicsContextHandle) {
    ::DeleteObject((HGDIOBJ)graphicsContextHandle);
    graphicsContextHandle = 0;
  }
#else // unix
#endif // flavor
}

// GraphicsContext::GraphicsContext() throw(UserInterfaceException) {
// }

GraphicsContext::Pen::Pen(PenStyle style, Color color, unsigned int width) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static const int NATIVE_STYLES[] = { // keep in sync with GraphicsContext::PenStyle
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

GraphicsContext::Brush::Brush(SystemColor color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static const int NATIVE_COLORS[] = {
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

GraphicsContext::Brush::Brush(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HBRUSH brush = ::CreateSolidBrush(color.value);
  assert(brush, UserInterfaceException(this));
  setHandle(brush);
#else // unix
#endif // flavor
}

GraphicsContext::Brush::Brush(unsigned int color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HBRUSH brush = ::CreateSolidBrush(color & 0xffffff);
  assert(brush, UserInterfaceException(this));
  setHandle(brush);
#else // unix
#endif // flavor
}

GraphicsContext::Font::Font(const String& name) throw(UserInterfaceException) {
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

void GraphicsContext::setPen(Pen pen) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  this->pen = pen; // put lock on pen
  HGDIOBJ handle = ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)pen.getHandle()
  );
#else // unix
#endif // flavor
}

void GraphicsContext::setBrush(Brush brush) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  this->brush = brush; // put lock on brush
  HGDIOBJ handle = ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)brush.getHandle()
  );
#else // unix
#endif // flavor
}

void GraphicsContext::setFont(Font font) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HGDIOBJ handle = ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)font.getHandle()
  );
#else // unix
#endif // flavor  
}

void GraphicsContext::setBackgroundMode(bool transparent) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetBkMode(
    (HDC)graphicsContextHandle,
    transparent ? TRANSPARENT : OPAQUE
  );
#else // unix
#endif // flavor
};

void GraphicsContext::setBackgroundColor(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetBkColor(
    (HDC)graphicsContextHandle,
    color.value
  );
#else // unix
#endif // flavor
};

void GraphicsContext::setTextColor(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetTextColor(
    (HDC)graphicsContextHandle,
    color.value
  );
#else // unix
#endif // flavor
};

void GraphicsContext::setTextAlignment(unsigned int alignment) throw(UserInterfaceException) {
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

// Dimension GraphicsContext::getDimension() const throw(UserInterfaceException) {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   RECT rect;
//   ::GetClientRect((HWND)drawableHandle, &rect);
//   return Region();
// #else // unix
// #endif // flavor
// }

void GraphicsContext::clear() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  //::GetClientRect((HWND)drawableHandle, &rect);
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
    (::Window)drawableHandle
  );
#endif // flavor
}

void GraphicsContext::clear(const Position& position, const Dimension& dimension) throw(UserInterfaceException) {
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
    (::Window)drawableHandle,
    position.getX(),
    position.getY(),
    dimension.getWidth(),
    dimension.getHeight(),
    False
  );
#endif // flavor
}

void GraphicsContext::point(const Position& position, unsigned int flags) throw(UserInterfaceException) {
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
    (::Window)drawableHandle,
    (GC)graphicsContextHandle,
    position.getX(),
    position.getY()
  );
#endif // flavor
}

// void GraphicsContext::points(const Array<Position>& position, unsigned int flags) throw(UserInterfaceException) {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
// #else // unix
//   XPoint* points = 0;
//   ::XDrawPoints(
//     (Display*)displayHandle,
//     (::Window)drawableHandle,
//     (GC)graphicsContextHandle,
//     points,
//     numberOfPoints,
//     CoordModeOrigin // mode
//   );
// #endif // flavor  
// }

void GraphicsContext::line(const Position& a, const Position& b, unsigned int flags) throw(UserInterfaceException) {
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
    (::Window)drawableHandle,
    (GC)graphicsContextHandle,
    a.getX(),
    a.getY(),
    b.getX(),
    b.getY()
  );
#endif // flavor  
}

// void GraphicsContext::polyline(const Array<Position>& points, unsigned int flags) throw(UserInterfaceException) {
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
// //     (::Window)drawableHandle,
// //     (GC)graphicsContextHandle,
// //     a.getX(),
// //     a.getY(),
// //     b.getX(),
// //     b.getY()
// //   );
// #endif // flavor  
// }

void GraphicsContext::arc(const Position& position, const Dimension& dimension, int start, int stop, unsigned int flags) throw(UserInterfaceException) {
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
  if (flags & GraphicsContext::FILLED) {
    ::XFillArc(
      (Display*)displayHandle,
      (::Window)drawableHandle,
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
      (::Window)drawableHandle,
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

// void GraphicsContext::arc(const Position& position, const Dimension& dimension, int start, int stop, unsigned int flags) throw(UserInterfaceException) {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
// assert(::AngleArc(
//   H(HDC)graphicsContextHandle,
//   position.getX(),
//   position.getY(),
//   radius,
//   angleOffset,
//   angleSpan
// );#else // unix
// #endif // flavor
// }

void GraphicsContext::rectangle(const Position& a, const Position& b, unsigned int flags) throw(UserInterfaceException) {
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
//   if (flags & GraphicsContext::FILLED) {
//     ::XFillRectangle(
//       (Display*)displayHandle,
//       (::Window)drawableHandle,
//       (GC)graphicsContextHandle,
//       position.getX(),
//       position.getY(),
//       dimension.getWidth(),
//       dimension.getHeight()
//     );
//   } else {
//     ::XDrawRectangle(
//       (Display*)displayHandle,
//       (::Window)drawableHandle,
//       (GC)graphicsContextHandle,
//       position.getX(),
//       position.getY(),
//       dimension.getWidth(),
//       dimension.getHeight()
//     );
//   }
#endif // flavor  
}

void GraphicsContext::rectangle(const Position& position, const Dimension& dimension, unsigned int flags) throw(UserInterfaceException) {
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
  if (flags & GraphicsContext::FILLED) {
    ::XFillRectangle(
      (Display*)displayHandle,
      (::Window)drawableHandle,
      (GC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight()
    );
  } else {
    ::XDrawRectangle(
      (Display*)displayHandle,
      (::Window)drawableHandle,
      (GC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      dimension.getWidth(),
      dimension.getHeight()
    );
  }
#endif // flavor  
}

void GraphicsContext::rectangle(const Position& a, const Position& b, Brush brush, unsigned int flags) throw(UserInterfaceException) {
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
  (::Window)drawableHandle,
  (GC)graphicsContextHandle,
  arcs, // XArc
  narcs // int
);

::XFillPolygon(
  (Display*)displayHandle,
  (::Window)drawableHandle,
  (GC)graphicsContextHandle,
  points,
  npoints,
  shape,
  mode
);

void GraphicsContext::rectangle(const Array<Region>& rectangles, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  ::XFillRectangles(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    (GC)graphicsContextHandle,
    rectangles,
    rectangles.getLength()
  );
#endif // flavor  
}
#endif

void GraphicsContext::ellipse(const Position& a, const Position& b, unsigned int flags) throw(UserInterfaceException) {
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

void GraphicsContext::pie(
  const Position& a, const Position& b,
  const Position& radialA, const Position& radialB,
  unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Pie(
      (HDC)graphicsContextHandle,
      a.getX(),
      a.getY(),
      b.getX(),
      b.getY(),
      radialA.getX(),
      radialA.getY(),
      radialB.getX(),
      radialB.getY()
    ),
    UserInterfaceException(this)
  );
#else // unix
#endif // flavor  
}

Dimension GraphicsContext::getDimensionOf(const String& text) const throw(UserInterfaceException) {
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
//      (HDC)graphicsContextHandle,
//   LPCTSTR lpszStr, // character string
//   int cchString,   // number of characters
//   int nMaxExtent,  // maximum width of formatted string
//   LPINT lpnFit,    // maximum number of characters
//   LPINT alpDx,     // array of partial string widths
//   LPSIZE lpSize    // string dimensions
// );

// TAG: need ucs2 method
void GraphicsContext::text(const Position& position, const Dimension& dimension, const String& text, unsigned int flags) throw(UserInterfaceException) {
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
    (::Window)drawableHandle,
    (GC)graphicsContextHandle,
    position.getX(),
    position.getY(),
    text.getElements(),
    text.getLength()
  );
#endif // flavor  
}

void GraphicsContext::putBitmap(
  const Position& position,
  const Dimension& dimension, Bitmap bitmap) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
//   ::XPutImage(
//     (Display*)displayHandle,
//     (Drawable)drawableHandle,
//     (GC)graphicsContextHandle,
//     bitmap.bitmapHandle,
//     0,
//     0,
//     position.getX(),
//     position.getY(),
//     dimension.getWidth(),
//     dimension.getHeight()
//   );
#endif // flavor  
}

Bitmap GraphicsContext::getBitmap(
  const Position& position,
  const Dimension& dimension) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return Bitmap();
#else // unix
//   XImage* nativeBitmap = ::XGetImage(
//     (Display*)displayHandle,
//     (Drawable)drawableHandle,
//     position.getX(),
//     position.getY(),
//     dimension.getWidth(),
//     dimension.getHeight(),
//     1, // TAG: fixme - depth of image (use depth of drawable?)
//     XYPixmap
//   );
  return Bitmap();
#endif // flavor  
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
