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
#include <base/ui/Bitmap.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Bitmap::Handle::~Handle() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (handle) {
    ::DeleteObject((HGDIOBJ)handle);
  }
#else // unix
  if (handle) {
    // TAG: fixme
  }
#endif // flavor
}

Bitmap::Bitmap(const Dimension& dimension, Format format, Encoding encoding, const void* data) throw(UserInterfaceException)
  : handle(0) {
  if (!dimension.isProper()) {
    return;
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HDC h = ::GetDCEx(0, 0, 0); // TAG: destroy this?
  HDC deviceContextHandle = ::CreateCompatibleDC(h);
  assert(deviceContextHandle, UserInterfaceException("Unable to create bitmap", this));
  HBITMAP bitmapHandle = ::CreateCompatibleBitmap((HDC)h, dimension.getWidth(), dimension.getHeight());
  if (!bitmapHandle) {
    ::DeleteDC((HDC)deviceContextHandle);
    throw UserInterfaceException("Unable to create bitmap", this);
  }
  HGDIOBJ previous = ::SelectObject((HDC)deviceContextHandle, (HGDIOBJ)bitmapHandle);
  if (!previous) {
    ::DeleteObject(bitmapHandle);
    ::DeleteDC(deviceContextHandle);
    throw UserInterfaceException("Unable to create bitmap", this);    
  } else {
    ::DeleteObject(previous); // TAG: is this required
  }
  
  assert((format == Bitmap::RGB) && (encoding == Bitmap::RGB_32), UserInterfaceException(this));
  
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
  
  assert(
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
#else // unix
  // TAG: fixme
#endif // flavor
}

#if 0
void Bitmap::decode(const Dimension& dimension, Format format, Encoding encoding, const void* data) throw() {
}

void Bitmap::encode(Format format, Encoding encoding, void* data) throw() {
}
#endif

Dimension Bitmap::getDimension() const throw(UserInterfaceException) {
  if (!handle.isValid()) {
    return Dimension(0, 0);
  }
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HGDIOBJ bitmapHandle = ::GetCurrentObject((HDC)handle->getHandle(), OBJ_BITMAP);
  BITMAP info;
  ::GetObject(
    (HBITMAP)bitmapHandle,
    sizeof(info),
    &info
  );
  return Dimension(info.bmWidth, info.bmHeight);
#else // unix
  return Dimension(0, 0);
  // TAG: fixme
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
