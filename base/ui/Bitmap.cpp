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
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#  include <base/platforms/os/unix/X11.h>
#  include <stdlib.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
template<>
class Backend<WindowImpl> {
public:

  static inline void* getDisplayHandle() noexcept
  {
    return WindowImpl::displayHandle;
  }
};
#endif

Bitmap::Handle::~Handle() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (handle) {
    ::DeleteObject((HGDIOBJ)handle);
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  if (handle) {
    XDestroyImage((XImage*)handle);
  }
#endif // flavor
}

Bitmap::Bitmap(
  const Dimension& dimension,
  Format format,
  Encoding encoding,
  const void* data)
  : handle(0) {
  if (!dimension.isProper()) {
    return;
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HDC h = ::GetDCEx(0, 0, 0); // TAG: destroy this?
  HDC deviceContextHandle = ::CreateCompatibleDC(h);
  bassert(
    deviceContextHandle,
    UserInterfaceException("Unable to create bitmap.", this)
  );
  HBITMAP bitmapHandle = ::CreateCompatibleBitmap(
    (HDC)h,
    dimension.getWidth(),
    dimension.getHeight()
  );
  if (!bitmapHandle) {
    ::DeleteDC((HDC)deviceContextHandle);
    throw UserInterfaceException("Unable to create bitmap.", this);
  }
  HGDIOBJ previous = ::SelectObject(
    (HDC)deviceContextHandle,
    (HGDIOBJ)bitmapHandle
  );
  if (!previous) {
    ::DeleteObject(bitmapHandle);
    ::DeleteDC(deviceContextHandle);
    throw UserInterfaceException("Unable to create bitmap.", this);    
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
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
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
  const void* data) noexcept {
}

void Bitmap::encode(Format format, Encoding encoding, void* data) noexcept {
}
#endif

// TAG: use Color
uint32 Bitmap::getPixel(const Position& position) const
{
  if (!handle.isValid()) {
    throw NullPointer(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0; // TAG: fixme
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  unsigned long pixel = XGetPixel(
    (XImage*)handle->getHandle(),
    position.getX(),
    position.getY()
  );
  return pixel;
#else
  return 0;
#endif // flavor
}

// TAG: use Color
void Bitmap::setPixel(
  const Position& position,
  uint32 value) const
{
  if (!handle.isValid()) {
    throw NullPointer(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: fixme
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XPutPixel(
    (XImage*)handle->getHandle(),
    position.getX(),
    position.getY(),
    value
  );
#endif // flavor
}

Dimension Bitmap::getDimension() const {
  if (!handle.isValid()) {
    return Dimension(0, 0);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
#elif defined(_COM_AZURE_DEV__BASE__USE_X11)
  XImage* image = (XImage*)handle->getHandle();
  return Dimension(image->width, image->height);
#else
  return Dimension();
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
