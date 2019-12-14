/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/ui/Pixmap.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#else // unix (X11)
#  include <base/platforms/os/unix/X11.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: support for capturing screen/window/...

void Pixmap::destroy() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)  
  if (graphicsContextHandle) {
    HGDIOBJ bitmap = ::SelectObject((HDC)graphicsContextHandle, (HGDIOBJ)0); // TAG: is this required
    ::DeleteObject(bitmap); // TAG: is this required
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

Pixmap::Pixmap(const Dimension& dimension, unsigned int flags) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HDC pixmap = ::CreateCompatibleDC(0 /*::GetDCEx(0, 0, 0)*/);
  bassert(pixmap, UserInterfaceException("Unable to create pixmap.", this));
  HBITMAP bitmap = ::CreateCompatibleBitmap((HDC)pixmap, dimension.getWidth(), dimension.getHeight());
  if (!bitmap) {
    ::DeleteDC((HDC)pixmap);
    throw UserInterfaceException("Unable to create pixmap.", this);
  }
  HGDIOBJ previous = ::SelectObject((HDC)pixmap, (HGDIOBJ)bitmap);
  if (!previous) {
    ::DeleteObject(bitmap);
    ::DeleteDC(pixmap);
    throw UserInterfaceException("Unable to create pixmap.", this);
  } else {
    ::DeleteObject(previous); // TAG: is this required
  }
  graphicsContextHandle = (void*)pixmap;
#else // unix
  // TAG: fixme
#endif // flavor
}

// Dimension Pixmap::getDimension() {
// #if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//   HGDIOBJ bitmap = ::GetCurrentObject((HDC)drawableHandle, OBJ_BITMAP);
//   // bitmap
//   return Dimension(0, 0);
// #else // unix
//   // TAG: fixme
//   return Dimension(0, 0);
// #endif // flavor
// }

void Pixmap::encode(Format format, Encoding encoding, void* data) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if ((format == Pixmap::RGB) && (encoding == Pixmap::RGB_32)) {
//     BITMAPINFOHEADER info;
//     info.biSize = sizeof(info);
//     info.biWidth = ?;
//     info.biHeight = ?;
//     info.biPlanes = 1;
//     info.biBitCount = 32;
//     info.biCompression = BI_RGB;
    
//     int ::GetDIBits(
//       (HDC)graphicsContextHandle,
//       (HBITMAP)bitmap,
//       0, // first line
//       cScanLines,   // number of scan lines to copy
//       lpvBits,    // array for bitmap bits
//       &info, // bitmap data buffer
//       DIB_RGB_COLORS
//     );
  }
#else // unix
  // TAG: fixme
#endif // flavor
}

// void* Pixmap::getData() noexcept {
// }

Pixmap::~Pixmap() noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#else // unix
#endif // flavor
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
