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
  if (handle) {
    ::DeleteObject((HGDIOBJ)handle);
    handle = 0;
  }
#else // unix
#endif // flavor
}

GraphicsContext::Pen::Pen(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HPEN pen = ::CreatePen(PS_SOLID, 1, color.getValue());
  assert(pen, UserInterfaceException(this));
  setHandle(pen);
#else // unix
#endif // flavor
}

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
    color.getValue()
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
  HBRUSH brush = ::CreateSolidBrush(color.getValue());
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

#if 0
int CALLBACK EnumFontFamExProc(
  ENUMLOGFONTEX* lpelfe, // logical-font data
  NEWTEXTMETRICEX* lpntme, // physical-font data
  DWORD fontType, // type of font
  LPARAM lParam // application-defined data
) {
}
#endif

Array<String> GraphicsContext::Font::getFonts() throw(UserInterfaceException) {
  Array<String> result;
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  LOGFONT logFont;
//   logFont.lfHeight = 0;
//   logFont.lfWidth = 0;
//   logFont.lfEscapement = 0;
//   logFont.lfOrientation = 0;
//   logFont.lfWeight = 0;
//   logFont.lfItalic = 0;
//   logFont.lfUnderline = 0;
//   logFont.lfStrikeOut = 0;
//   logFont.lfCharSet = 0;
//   logFont.lfOutPrecision = 0;
//   logFont.lfClipPrecision = 0;
//   logFont.lfQuality = ;
//   logFont.lfPitchAndFamily;
//   logFont.lfFaceName = 0;
//   int ::EnumFontFamiliesEx(
//     (HDC)::GetDCEx(0, 0, 0),
//     &logfont,
//     lpEnumFontFamExProc,
//     &temp,
//     0
//   );
#else // unix
#endif // flavor
  return result;
}

GraphicsContext::Font::Font(const String& name, unsigned short height, FontWeight weight, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static int NATIVE_WEIGHTS[] = {
    FW_THIN, // THIN
    FW_LIGHT, // LIGHT
    FW_NORMAL, // NORMAL
    FW_BOLD, // BOLD
    FW_HEAVY // HEAVY
  };
  HFONT font = ::CreateFont(
    height, // height of font
    0, // average character width
    0, // angle of escapement
    0, // base-line orientation angle
    NATIVE_WEIGHTS[weight], // font weight
    (flags & GraphicsContext::ITALIC) ? TRUE : FALSE, // italic
    (flags & GraphicsContext::UNDERLINE) ? TRUE : FALSE, // underline
    (flags & GraphicsContext::STRIKE_OUT) ? TRUE : FALSE, // strikeout
    DEFAULT_CHARSET, // character set identifier
    0, // output precision
    CLIP_DEFAULT_PRECIS, // clipping precision
    ANTIALIASED_QUALITY, // quality
    DEFAULT_PITCH, // pitch and family
    name.getElements() // typeface name
  );
  assert(font, UserInterfaceException(this));
  setHandle(font);
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
  ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)pen.getHandle()
  );
#else // unix
#endif // flavor
}

void GraphicsContext::setBrush(Brush brush) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  this->brush = brush; // put lock on brush
  ::SelectObject(
    (HDC)graphicsContextHandle,
    (HGDIOBJ)brush.getHandle()
  );
#else // unix
#endif // flavor
}

void GraphicsContext::setFont(Font font) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SelectObject(
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
    color.getValue()
  );
#else // unix
#endif // flavor
};

void GraphicsContext::setTextColor(Color color) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetTextColor(
    (HDC)graphicsContextHandle,
    color.getValue()
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

void GraphicsContext::clear() throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = dimension.getWidth(); // do not subtract 1
  rect.bottom = dimension.getHeight(); // do not subtract 1
  assert(
    ::FillRect(
      (HDC)graphicsContextHandle,
      &rect,
      (HBRUSH)brush.getHandle()
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

void GraphicsContext::setPixel(const Position& position, Color color, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::SetPixel(
      (HDC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      color.getValue()
    ) != (COLORREF)(-1),
    UserInterfaceException(this)
  );
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

Color GraphicsContext::getPixel(const Position& position) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  COLORREF result = ::GetPixel(
    (HDC)graphicsContextHandle,
    position.getX(),
    position.getY()
  );
  assert(result != CLR_INVALID, UserInterfaceException(this));
  return Color((unsigned int)result);
#else // unix
  // TAG: fixme
#endif // flavor
}

void GraphicsContext::setPixels(const Array<Position>& positions, Color color, unsigned int flags) throw(UserInterfaceException) {
// ::XInitImage, ::XCreateImage, XGetPixel, XPutPixel, XSubImage, XAddPixel, ::XDestroyImage
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Array<Position>::ReadIterator i = positions.getBeginIterator();
  const Array<Position>::ReadIterator end = positions.getEndIterator();
  while (i != end) {
    assert(
      ::SetPixel(
        (HDC)graphicsContextHandle,
        i->getX(),
        i->getY(),
        color.getValue()
      ) != (COLORREF)(-1),
      UserInterfaceException(this)
    );
  }
#else // unix
  // TAG: fixme
//   XPoint* points = 0;
//   ::XDrawPoints(
//     (Display*)displayHandle,
//     (::Window)drawableHandle,
//     (GC)graphicsContextHandle,
//     points,
//     numberOfPoints,
//     CoordModeOrigin // mode
//   );
#endif // flavor
}

void GraphicsContext::moveTo(const Position& position) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::MoveToEx(
      (HDC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      0
    ),
    UserInterfaceException(this)
  );
#else // unix
  // TAG: fixme
#endif // flavor  
}

void GraphicsContext::lineTo(const Position& position, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::LineTo(
      (HDC)graphicsContextHandle,
      position.getX(),
      position.getY()
    ),
    UserInterfaceException(this)
  );
#else // unix
  // TAG: fixme
#endif // flavor  
}

void GraphicsContext::line(const Position& upperLeft, const Position& lowerRight, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  POINT points[3];
  points[0].x = upperLeft.getX();
  points[0].y = upperLeft.getY();
  points[1].x = lowerRight.getX();
  points[1].y = lowerRight.getY();
  points[2] = points[1];
  assert(
    ::Polyline(
      (HDC)graphicsContextHandle,
      points,
      3
    ),
    UserInterfaceException(this)
  );
#else // unix
  ::XDrawLine(
    (Display*)displayHandle,
    (::Window)drawableHandle,
    (GC)graphicsContextHandle,
    upperLeft.getX(),
    upperLeft.getY(),
    lowerRight.getX(),
    lowerRight.getY()
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
// //     upperLeft.getX(),
// //     upperLeft.getY(),
// //     lowerRight.getX(),
// //     lowerRight.getY()
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

void GraphicsContext::rectangle(const Position& upperLeft, const Position& lowerRight, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Rectangle(
      (HDC)graphicsContextHandle,
      upperLeft.getX(),
      upperLeft.getY(),
      lowerRight.getX(),
      lowerRight.getY()
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

void GraphicsContext::rectangle(const Position& upperLeft, const Position& lowerRight, Brush brush, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = upperLeft.getX();
  rect.top = upperLeft.getY();
  rect.right = lowerRight.getX() + 1;
  rect.bottom = lowerRight.getY() + 1;
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

void GraphicsContext::rectangle(const Position& position, const Dimension& dimension, Brush brush, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  RECT rect;
  rect.left = position.getX();
  rect.top = position.getY();
  rect.right = rect.left + dimension.getWidth(); // do not subtract 1
  rect.bottom = rect.top + dimension.getHeight(); // do not subtract 1
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

void GraphicsContext::ellipse(const Position& upperLeft, const Position& lowerRight, unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Ellipse(
      (HDC)graphicsContextHandle,
      upperLeft.getX(),
      upperLeft.getY(),
      lowerRight.getX(),
      lowerRight.getY()
    ),
    UserInterfaceException(this)
  );
#else // unix
#endif // flavor  
}

void GraphicsContext::ellipse(const Position& position, const Dimension& dimension, unsigned int flags) throw(UserInterfaceException) {
  if (!dimension.isProper()) {
    return;
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Ellipse(
      (HDC)graphicsContextHandle,
      position.getX(),
      position.getY(),
      position.getX() + dimension.getWidth() - 1,
      position.getY() + dimension.getHeight() - 1
    ),
    UserInterfaceException(this)
  );
#else // unix
#endif // flavor  
}

void GraphicsContext::pie(
  const Position& upperLeft,
  const Position& lowerRight,
  const Position& radialA,
  const Position& radialB,
  unsigned int flags) throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  assert(
    ::Pie(
      (HDC)graphicsContextHandle,
      upperLeft.getX(),
      upperLeft.getY(),
      lowerRight.getX(),
      lowerRight.getY(),
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

unsigned int GraphicsContext::getWidthOf(char ch) const throw(UserInterfaceException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  INT width;
  assert(
    ::GetCharWidth32(
      (HDC)graphicsContextHandle,
      ch,
      ch,
      &width
    ),
    UserInterfaceException(this)
  );
  return width;
#else // unix
  return 0; // TAG: fixme
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
  const Dimension& dimension,
  const Bitmap& bitmap) throw(UserInterfaceException) {
  // TAG: COPY, AND, OR, XOR, INVERT
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (bitmap.handle.isValid()) {
    assert(
      ::BitBlt(
        (HDC)graphicsContextHandle,
        position.getX(),
        position.getY(),
        dimension.getWidth(),
        dimension.getHeight(),
        (HDC)bitmap.handle->getHandle(),
        0, // x
        0, // y
        SRCCOPY
      ),
      UserInterfaceException(this)
    );
  }
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
  HDC deviceContext = ::CreateCompatibleDC((HDC)graphicsContextHandle);
  assert(deviceContext, UserInterfaceException(this));
  HBITMAP bitmap = ::CreateCompatibleBitmap((HDC)deviceContext, dimension.getWidth(), dimension.getHeight());
  if (!bitmap) {
    ::DeleteDC((HDC)deviceContext);
    throw UserInterfaceException(this);
  }
  HGDIOBJ previous = ::SelectObject((HDC)deviceContext, (HGDIOBJ)bitmap);
  if (!previous) {
    ::DeleteObject(bitmap);
    ::DeleteDC(deviceContext);
    throw UserInterfaceException(this);    
  } else {
    ::DeleteObject(previous); // TAG: is this required
  }
  Bitmap result;
  result.handle = new Bitmap::Handle(deviceContext);
  return result;
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

GraphicsContext::~GraphicsContext() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
