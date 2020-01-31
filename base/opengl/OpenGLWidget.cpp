/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/opengl/OpenGLWidget.h>
#include <base/platforms/backend/WindowImpl.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <base/platforms/win32/GDI.h>
#else // unix
#  include <base/platforms/os/unix/GLX.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void OpenGLWidget::destroy() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)  
  native::GDI::wglMakeCurrent(0, 0); // deselect current rendering context
  if (renderingContextHandle) {
    native::GDI::wglDeleteContext((HGLRC)renderingContextHandle);
    renderingContextHandle = 0;
  }
  if (WindowImpl::graphicsContextHandle) {
    ::DeleteDC((HDC)WindowImpl::graphicsContextHandle);
  }
  if (drawableHandle) {
    // nothing to destroy
  }
  // screenHandle not used
#else // unix
  native::GLX::glXMakeCurrent(
    (Display*)Backend<WindowImpl>::getDisplay(),
    (native::GLX::GLXDrawable)None,
    (native::GLX::GLXContext)0
  );
  if (renderingContextHandle) {
    native::GLX::glXDestroyContext(
      (Display*)displayHandle,
      (native::GLX::GLXContext)renderingContextHandle
    );
    renderingContextHandle = 0;
  }
  if (WindowImpl::graphicsContextHandle) {
    // nothing to destroy
  }
  if (drawableHandle) {
    // GLX 1.3 native::GLX::glXDestroyWindow((Display*)displayHandle, (native::GLX::GLXWindow)drawableHandle);
    ::XDestroyWindow((Display*)displayHandle, (::Window)drawableHandle);
  }
  if (screenHandle) {
    // nothing to destroy
  }
#endif // flavor
  WindowImpl::destroy();
}

nothing OpenGLWidget::initialize(const Format& format)
{
  OpenGLContextImpl::loadModule();
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)  
  OpenGLContextImpl::graphicsContextHandle = WindowImpl::graphicsContextHandle; // TAG: fixme
  
  int formatId = 0;
  unsigned int numberOfFormats = ::DescribePixelFormat((HDC)WindowImpl::graphicsContextHandle, 0, 0, 0);
  PIXELFORMATDESCRIPTOR pfd;
  for (unsigned int i = 1; i <= numberOfFormats; ++i) {
    ::DescribePixelFormat(
      (HDC)WindowImpl::graphicsContextHandle,
      i,
      sizeof(PIXELFORMATDESCRIPTOR),
      &pfd
    );
    
    bool candidate = (pfd.dwFlags & PFD_DRAW_TO_WINDOW) && (pfd.dwFlags & PFD_SUPPORT_OPENGL);
    if (format.flags & OpenGLContextImpl::COLOR_INDEXED) {
      candidate &= pfd.iPixelType == PFD_TYPE_COLORINDEX;
    }
    if (format.flags & OpenGLContextImpl::RGB) {
      candidate &= pfd.iPixelType == PFD_TYPE_RGBA;
    }
    if (format.flags & OpenGLContextImpl::GENERIC) {
      candidate &= (pfd.dwFlags & PFD_GENERIC_FORMAT) != 0;
    }
    if (format.flags & OpenGLContextImpl::DOUBLE_BUFFERED) {
      candidate &= (pfd.dwFlags & PFD_DOUBLEBUFFER) != 0;
    }
    if (format.flags & OpenGLContextImpl::STEREO) {
      candidate &= (pfd.dwFlags & PFD_STEREO) != 0;
    }
    candidate &= pfd.cColorBits == format.colorBits;
    candidate &= pfd.cRedBits == format.redBits;
    candidate &= pfd.cGreenBits == format.greenBits;
    candidate &= pfd.cBlueBits == format.blueBits;
    candidate &= pfd.cAlphaBits == format.alphaBits;
    candidate &= pfd.cAccumBits == format.accumulatorBits;
    candidate &= pfd.cAccumRedBits == format.accumulatorRedBits;
    candidate &= pfd.cAccumGreenBits == format.accumulatorGreenBits;
    candidate &= pfd.cAccumBlueBits == format.accumulatorBlueBits;
    candidate &= pfd.cAccumAlphaBits == format.accumulatorAlphaBits;
    candidate &= pfd.cDepthBits == format.depthBits;
    candidate &= pfd.cStencilBits == format.stencilBits;
    candidate &= pfd.cAuxBuffers == format.auxBuffers;
    if (candidate) {
      formatId = i;
      break;
    }
  }
  bassert(formatId != 0, OpenGLException("Requested format not supported.", this));
  
  OpenGLContextImpl::flags = 0;
  OpenGLContextImpl::flags |= (pfd.iPixelType == PFD_TYPE_COLORINDEX) ? OpenGLContextImpl::COLOR_INDEXED : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_DOUBLEBUFFER) ? OpenGLContextImpl::DOUBLE_BUFFERED : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_STEREO) ? OpenGLContextImpl::STEREO : 0;
  // OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_SWAP_LAYER_BUFFERS) ? OpenGLContextImpl::SWAP_LAYER : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_GENERIC_ACCELERATED) ? OpenGLContextImpl::DIRECT : 0;
  OpenGLContextImpl::flags |= (pfd.dwFlags & PFD_GENERIC_FORMAT) ? OpenGLContextImpl::GENERIC : 0;
  
  redBits = pfd.cRedBits;
  redShift = pfd.cRedShift;
  greenBits = pfd.cGreenBits;
  greenShift = pfd.cGreenShift;
  blueBits = pfd.cBlueBits;
  blueShift = pfd.cBlueShift;
  alphaBits = pfd.cAlphaBits;
  alphaShift = pfd.cAlphaShift;
  accumulatorBits = pfd.cAccumBits;
  accumulatorRedBits = pfd.cAccumRedBits;
  accumulatorGreenBits = pfd.cAccumGreenBits;
  accumulatorBlueBits = pfd.cAccumBlueBits;
  accumulatorAlphaBits = pfd.cAccumAlphaBits;
  depthBits = pfd.cDepthBits;
  stencilBits = pfd.cStencilBits;
  auxBuffers = pfd.cAuxBuffers;
  numberOfOverlayPlanes = (pfd.bReserved >> 0) & 0xf;
  numberOfUnderlayPlanes = (pfd.bReserved >> 4) & 0xf;
  
  if (!::SetPixelFormat((HDC)WindowImpl::graphicsContextHandle, formatId, &pfd)) {
    _throw OpenGLException("Unable to set format.", this);
  }
  if (!(renderingContextHandle = native::GDI::wglCreateContext(
          (HDC)WindowImpl::graphicsContextHandle))) {
    _throw OpenGLException("Unable to create rendering context.", this);
  }
  if (!native::GDI::wglMakeCurrent(
        (HDC)WindowImpl::graphicsContextHandle, (HGLRC)renderingContextHandle)) {
    _throw OpenGLException("Invalid rendering context.", this);
  }
#else // unix
  // TAG: fixme
#endif // flavor
  return nothing();
}

OpenGLWidget::OpenGLWidget(Window& owner, const Format& format)
  : Widget(owner),
    OpenGLContextImpl(),
    prefixInitialization(initialize(format))
{
  construct();
  invalidate();
}

void OpenGLWidget::onDisplay() noexcept
{
  openGL.glClearColor(0.0, 0.0, 0.0, 1.0);
  openGL.glClear(OpenGL::COLOR_BUFFER_BIT);
  openGL.glFlush();
  swap();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
