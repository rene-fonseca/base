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
#include <base/ui/Bitmap.h>
#include <base/ui/WindowImpl.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
#else
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#  include <stdlib.h>
#endif
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
template<>
class Backend<WindowImpl> {
public:

  static inline void* getDisplayHandle() throw() {
    return WindowImpl::displayHandle;
  }
};
#endif

Bitmap::Handle::~Handle() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (handle) {
    ::DeleteObject((HGDIOBJ)handle);
  }
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
#else // unix
  if (handle) {
    XDestroyImage((XImage*)handle);
  }
#endif // flavor
}

Bitmap::Bitmap(
  const Dimension& dimension,
  Format format,
  Encoding encoding,
  const void* data) throw(UserInterfaceException)
  : handle(0) {
  if (!dimension.isProper()) {
    return;
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HDC h = ::GetDCEx(0, 0, 0); // TAG: destroy this?
  HDC deviceContextHandle = ::CreateCompatibleDC(h);
  bassert(
    deviceContextHandle,
    UserInterfaceException("Unable to create bitmap", this)
  );
  HBITMAP bitmapHandle = ::CreateCompatibleBitmap(
    (HDC)h,
    dimension.getWidth(),
    dimension.getHeight()
  );
  if (!bitmapHandle) {
    ::DeleteDC((HDC)deviceContextHandle);
    throw UserInterfaceException("Unable to create bitmap", this);
  }
  HGDIOBJ previous = ::SelectObject(
    (HDC)deviceContextHandle,
    (HGDIOBJ)bitmapHandle
  );
  if (!previous) {
    ::DeleteObject(bitmapHandle);
    ::DeleteDC(deviceContextHandle);
    throw UserInterfaceException("Unable to create bitmap", this);    
  } else {
    ::DeleteObject(previous); // TAG: is this required
  }
  
  bassert(
    (format == Bitmap::RGB) && (encoding == Bitmap::RGB_32),
    UserInterfaceException(this)
  );
  
  BITMAPINFO info;
  info.bmiHeader.biSize = sizeof(info.bmiHeader);
  info.bmiHeader.biWidth = dimension.getWidth();
  info.bmiHeader.biHeight = dimension.getHeight();
  info.bmiHeader.biPlanes = 1;
  info.bmiHeader.biBitCount = 32;
  info.bmiHeader.biCompression = BI_RGB;
  info.bmiHeader.biSizeImage = 0;
  info.bmiHeader.biXPelsPerMeter = 1;
  info.bmiHeader.biYPelsPerMeter = 1;
  info.bmiHeader.biClrUsed = 0;
  info.bmiHeader.biClrImportant = 0;
  
  bassert(
    ::SetDIBits(
      (HDC)deviceContextHandle,
      (HBITMAP)bitmapHandle,
      0,
      dimension.getHeight(),
      data,
      &info,
      DIB_RGB_COLORS
    ),
    UserInterfaceException(this)
  );
  handle = new Handle(deviceContextHandle);
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
#else // unix
  bassert( // TAG: what is the dimension limit
    (dimension.getWidth() < 65536) && (dimension.getHeight() < 65536),
    UserInterfaceException(this)
  );
  bassert(
    (format == Bitmap::RGB) && (encoding == Bitmap::RGB_32),
    UserInterfaceException(this)
  );
  
  Display* display = (Display*)Backend<WindowImpl>::getDisplayHandle();
  Visual* visual = XDefaultVisual(
    display,
    XDefaultScreen(display)
  );

  // TAG: depth can be: 8 (1 byte), 16 (2 bytes), 24 (4 bytes)
  
  void* newData = malloc(4 * dimension.getSize());
  copy<uint8>((uint8*)newData, (const uint8*)data, 4 * dimension.getSize());
  XImage* image = XCreateImage(
    display,
    visual,
    24, // depth
    ZPixmap, // format - XYBitmap (1-bit), XYPixmap (R,G,B planes), or ZPixmap (RGB pixels)
    0, // offset
    (char*)newData, // data
    dimension.getWidth(),
    dimension.getHeight(),
    8, // pad
    4 * dimension.getWidth() // bytes per line
  );
  handle = new Handle(image);
#endif // flavor
}

#if 0
void Bitmap::decode(
  const Dimension& dimension,
  Format format,
  Encoding encoding,
  const void* data) throw() {
}

void Bitmap::encode(Format format, Encoding encoding, void* data) throw() {
}
#endif

// TAG: use Color
uint32 Bitmap::getPixel(
  const Position& position) const throw(UserInterfaceException) {
  bassert(handle.isValid(), NullPointer(this));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return 0; // TAG: fixme
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
  return 0;
#else // unix
  unsigned long pixel = XGetPixel(
    (XImage*)handle->getHandle(),
    position.getX(),
    position.getY()
  );
  return pixel;
#endif // flavor
}

// TAG: use Color
void Bitmap::setPixel(
  const Position& position,
  uint32 value) const throw(UserInterfaceException) {
  bassert(handle.isValid(), NullPointer(this));
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // TAG: fixme
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
#else // unix
  XPutPixel(
    (XImage*)handle->getHandle(),
    position.getX(),
    position.getY(),
    value
  );
#endif // flavor
}

Dimension Bitmap::getDimension() const throw(UserInterfaceException) {
  if (!handle.isValid()) {
    return Dimension(0, 0);
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HGDIOBJ bitmapHandle = ::GetCurrentObject(
    (HDC)handle->getHandle(),
    OBJ_BITMAP
  );
  BITMAP info;
  ::GetObject(
    (HBITMAP)bitmapHandle,
    sizeof(info),
    &info
  );
  return Dimension(info.bmWidth, info.bmHeight);
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)
  return Dimension();
#else // unix
  XImage* image = (XImage*)handle->getHandle();
  return Dimension(image->width, image->height);
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
