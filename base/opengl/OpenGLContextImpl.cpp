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
#include <base/opengl/OpenGLContextImpl.h>
#include <base/ui/WindowImpl.h>
#include <base/platforms/backend/WindowImpl.h>
#include <base/concurrency/SpinLock.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/win32/GDI.cpp>
#else // unix
#  include <base/platforms/os/unix/GLX.cpp>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void OpenGLContextImpl::loadModule() throw(OpenGLException) {
  // TAG: never release ui component?
  assert(
    Backend<WindowImpl>::loadModule(true),
    OpenGLException("Unable to load module", Type::getType<OpenGLContextImpl>())
  );
  static SpinLock spinLock;
  spinLock.exclusiveLock();
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  bool success = native::GDI::import();
#else // unix
  bool success = native::GLX::import();
#endif // flavor
  spinLock.releaseLock();
  assert(
    success,
    OpenGLException("Unable to load module", Type::getType<OpenGLContextImpl>())
  );
}

OpenGLContextImpl::OpenGLContextImpl() throw(OpenGLException) {
}

String OpenGLContextImpl::getGLClientVendor() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String(); // TAG: fixme
#else // unix
  return native::GLX::glXGetClientString((Display*)Backend<WindowImpl>::getDisplay(), native::GLX::VENDOR);
#endif // flavor
}

String OpenGLContextImpl::getGLClientRelease() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String(); // TAG: fixme
#else // unix
  return native::GLX::glXGetClientString((Display*)Backend<WindowImpl>::getDisplay(), native::GLX::VERSION);
#endif // flavor
}

String OpenGLContextImpl::getGLClientExtensions() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String(); // TAG: fixme
#else // unix
  return native::GLX::glXGetClientString((Display*)Backend<WindowImpl>::getDisplay(), native::GLX::EXTENSIONS);
#endif // flavor
}

String OpenGLContextImpl::getGLServerVendor() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String(); // TAG: fixme
#else // unix
  void* screenHandle = 0; // TAG: fixme move method to OpenGLWindowContext?
  return native::GLX::glXQueryServerString(
    (Display*)Backend<WindowImpl>::getDisplay(),
    ::XScreenNumberOfScreen((Screen*)screenHandle),
    native::GLX::VENDOR
  );
#endif // flavor
}

String OpenGLContextImpl::getGLServerRelease() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String(); // TAG: fixme
#else // unix
  void* screenHandle = 0; // TAG: fixme move method to OpenGLWindowContext?
  return native::GLX::glXQueryServerString(
    (Display*)Backend<WindowImpl>::getDisplay(),
    ::XScreenNumberOfScreen((Screen*)screenHandle),
    native::GLX::VERSION
  );
#endif // flavor
}

String OpenGLContextImpl::getGLServerExtensions() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return String(); // TAG: fixme
#else // unix
  void* screenHandle = 0; // TAG: fixme move method to OpenGLWindowContext?
  return native::GLX::glXQueryServerString(
    (Display*)Backend<WindowImpl>::getDisplay(),
    ::XScreenNumberOfScreen((Screen*)screenHandle),
    native::GLX::EXTENSIONS
  );
#endif // flavor
}

// bool OpenGLContextImpl::isDirect() const throw(OpenGLException) {
// #if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   return true; // TAG: unless pixmap
// #else // unix
//   return native::GLX::glXIsDirect(
//     (Display*)Backend<WindowImpl>::getDisplay(),
//     (native::GLX::GLXContext)renderingContextHandle
//   ) == True;
// #endif // flavor
// }

bool OpenGLContextImpl::hasCurrent() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return native::GDI::wglGetCurrentContext() != 0;
#else // unix
  return native::GLX::glXGetCurrentContext() != 0;
#endif // flavor
}

bool OpenGLContextImpl::isCurrent() const throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return native::GDI::wglGetCurrentContext() == (HGLRC)renderingContextHandle;
#else // unix
  return native::GLX::glXGetCurrentContext() == (native::GLX::GLXContext)renderingContextHandle;
#endif // flavor
}

void OpenGLContextImpl::makeCurrent() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  ::SetLastError(0);
  native::GDI::wglMakeCurrent((HDC)graphicsContextHandle, (HGLRC)renderingContextHandle);
  fout << MESSAGE("Failed to make current: ") << ::GetLastError() << ENDL;
  assert(
    native::GDI::wglMakeCurrent((HDC)graphicsContextHandle, (HGLRC)renderingContextHandle),
    OpenGLException(this)
  );
#else // unix
  assert(
    native::GLX::glXMakeCurrent(
      (Display*)Backend<WindowImpl>::getDisplay(),
      (native::GLX::GLXDrawable)drawableHandle,
      (native::GLX::GLXContext)renderingContextHandle
    ) == True,
    OpenGLException(this)
  );
// GLX 1.3
//   Bool result = native::GLX::glXMakeContextCurrent(
//     (Display*)Backend<WindowImpl>::getDisplay(),
//     (native::GLX::GLXDrawable)drawableHandle,
//     (native::GLX::GLXDrawable)drawableHandle,
//     (native::GLX::GLXContext)renderingContextHandle
//   );
#endif // flavor
}

void OpenGLContextImpl::deselect() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  native::GDI::wglMakeCurrent(0, 0);
#else // unix
  assert(
    native::GLX::glXMakeCurrent(
      (Display*)Backend<WindowImpl>::getDisplay(),
      (native::GLX::GLXDrawable)None,
      (native::GLX::GLXContext)0
    ) == True,
    OpenGLException(this)
  );
// GLX 1.3
//   Bool result = native::GLX::glXMakeContextCurrent(
//     (Display*)Backend<WindowImpl>::getDisplay(),
//     None,
//     None,
//     0
//   );
#endif // flavor
}

void OpenGLContextImpl::swap() throw(OpenGLException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  native::GDI::wglSwapBuffers((HDC)graphicsContextHandle); // ignore errors
//   assert(
//     native::GDI::wglSwapBuffers((HDC)graphicsContextHandle),
//     OpenGLException("Unable to swap buffers", this)
//   );
#else // unix
  native::GLX::glXSwapBuffers((Display*)Backend<WindowImpl>::getDisplay(), (native::GLX::GLXDrawable)drawableHandle);
#endif // flavor
}

void OpenGLContextImpl::swap(int plane) throw(OutOfRange, OpenGLException) {
  assert(
    (plane >= -static_cast<int>(numberOfUnderlayPlanes)) && (plane <= numberOfOverlayPlanes),
    OutOfRange(this)
  );
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)  
  static const UINT NATIVE_PLANES[15 - (-15) + 1] = {
    WGL_SWAP_UNDERLAY15,
    WGL_SWAP_UNDERLAY14,
    WGL_SWAP_UNDERLAY13,
    WGL_SWAP_UNDERLAY12,
    WGL_SWAP_UNDERLAY11,
    WGL_SWAP_UNDERLAY10,
    WGL_SWAP_UNDERLAY9,
    WGL_SWAP_UNDERLAY8,
    WGL_SWAP_UNDERLAY7,
    WGL_SWAP_UNDERLAY6,
    WGL_SWAP_UNDERLAY5,
    WGL_SWAP_UNDERLAY4,
    WGL_SWAP_UNDERLAY3,
    WGL_SWAP_UNDERLAY2,
    WGL_SWAP_UNDERLAY1,
    WGL_SWAP_MAIN_PLANE,
    WGL_SWAP_OVERLAY1,
    WGL_SWAP_OVERLAY2,
    WGL_SWAP_OVERLAY3,
    WGL_SWAP_OVERLAY4,
    WGL_SWAP_OVERLAY5,
    WGL_SWAP_OVERLAY6,
    WGL_SWAP_OVERLAY7,
    WGL_SWAP_OVERLAY8,
    WGL_SWAP_OVERLAY9,
    WGL_SWAP_OVERLAY10,
    WGL_SWAP_OVERLAY11,
    WGL_SWAP_OVERLAY12,
    WGL_SWAP_OVERLAY13,
    WGL_SWAP_OVERLAY14,
    WGL_SWAP_OVERLAY15
  };
  assert(
    native::GDI::wglSwapLayerBuffers((HDC)graphicsContextHandle, NATIVE_PLANES[plane + 15]),
    OpenGLException("Unable to swap plane", this)
  );
#else // unix
  // TAG: fixme
  // TAG: lookup context in lot
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
