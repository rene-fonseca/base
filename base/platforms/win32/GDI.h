/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/platforms/features.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__WIN32)
#  error native GDI interface requires win32 flavor
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#endif // flavor

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define CALL_GDI __stdcall
#else
#  define CALL_GDI
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace native {

  namespace GDI {

    enum {
      WGL_NUMBER_PIXEL_FORMATS_ARB = 0x2000,
      WGL_DRAW_TO_WINDOW_ARB = 0x2001,
      WGL_DRAW_TO_BITMAP_ARB = 0x2002,
      WGL_ACCELERATION_ARB = 0x2003,
      WGL_NEED_PALETTE_ARB = 0x2004,
      WGL_NEED_SYSTEM_PALETTE_ARB = 0x2005,
      WGL_SWAP_LAYER_BUFFERS_ARB = 0x2006,
      WGL_SWAP_METHOD_ARB = 0x2007,
      WGL_NUMBER_OVERLAYS_ARB = 0x2008,
      WGL_NUMBER_UNDERLAYS_ARB = 0x2009,
      WGL_TRANSPARENT_ARB = 0x200A,
      WGL_TRANSPARENT_RED_VALUE_ARB = 0x2037,
      WGL_TRANSPARENT_GREEN_VALUE_ARB = 0x2038,
      WGL_TRANSPARENT_BLUE_VALUE_ARB = 0x2039,
      WGL_TRANSPARENT_ALPHA_VALUE_ARB = 0x203a,
      WGL_TRANSPARENT_INDEX_VALUE_ARB = 0x203b,
      WGL_SHARE_DEPTH_ARB = 0x200c,
      WGL_SHARE_STENCIL_ARB = 0x200d,
      WGL_SHARE_ACCUM_ARB = 0x200e,
      WGL_SUPPORT_GDI_ARB = 0x200f,
      WGL_SUPPORT_OPENGL_ARB = 0x2010,
      WGL_DOUBLE_BUFFER_ARB = 0x2011,
      WGL_STEREO_ARB = 0x2012,
      WGL_PIXEL_TYPE_ARB = 0x2013,
      WGL_COLOR_BITS_ARB = 0x2014,
      WGL_RED_BITS_ARB = 0x2015,
      WGL_RED_SHIFT_ARB = 0x2016,
      WGL_GREEN_BITS_ARB = 0x2017,
      WGL_GREEN_SHIFT_ARB = 0x2018,
      WGL_BLUE_BITS_ARB = 0x2019,
      WGL_BLUE_SHIFT_ARB = 0x201a,
      WGL_ALPHA_BITS_ARB = 0x201b,
      WGL_ALPHA_SHIFT_ARB = 0x201c,
      WGL_ACCUM_BITS_ARB = 0x201d,
      WGL_ACCUM_RED_BITS_ARB = 0x201e,
      WGL_ACCUM_GREEN_BITS_ARB = 0x201f,
      WGL_ACCUM_BLUE_BITS_ARB = 0x2020,
      WGL_ACCUM_ALPHA_BITS_ARB = 0x2021,
      WGL_DEPTH_BITS_ARB = 0x2022,
      WGL_STENCIL_BITS_ARB = 0x2023,
      WGL_AUX_BUFFERS_ARB = 0x2024,

      WGL_NO_ACCELERATION_ARB = 0x2025,
      WGL_GENERIC_ACCELERATION_ARB = 0x2026,
      WGL_FULL_ACCELERATION_ARB = 0x2027,
      WGL_SWAP_EXCHANGE_ARB = 0x2028,
      WGL_SWAP_COPY_ARB = 0x2029,
      WGL_SWAP_UNDEFINED_ARB = 0x202a,
      WGL_TYPE_RGBA_ARB = 0x202b,
      WGL_TYPE_COLORINDEX_ARB = 0x202c
    };
    
    typedef void (CALL_GDI *Function)();
    
    struct FunctionDescriptor {
      const char* symbol;
      Function* function;
    };
    
    extern HGLRC (CALL_GDI *wglCreateContext)(HDC hdc);
    extern HGLRC (CALL_GDI *wglCreateLayerContext)(HDC hdc, int plane);
    extern BOOL (CALL_GDI *wglDeleteContext)(HGLRC hglrc);
    extern HGLRC (CALL_GDI *wglGetCurrentContext)();
    extern HDC (CALL_GDI *wglGetCurrentDC)();
    extern PROC (CALL_GDI *wglGetProcAddress)(LPCSTR lpszProc);
    extern BOOL (CALL_GDI *wglMakeCurrent)(HDC hdc, HGLRC hglrc);
    extern BOOL (CALL_GDI *wglSwapBuffers)(HDC hdc);
    extern BOOL (CALL_GDI *wglSwapLayerBuffers)(HDC hdc, UINT planes);
    extern BOOL (CALL_GDI *wglDescribeLayerPlane)(HDC hdc, int pixelFormat, int layerPlane, UINT bytes, LPLAYERPLANEDESCRIPTOR lpd);
    
    bool import() noexcept;
  };
  
}; // end of native namespace

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
