/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/win32/GDI.h>
#include <base/dl/DynamicLinker.h>

// TAG: put in static description
#define _COM_AZURE_DEV__BASE__GDI_LIBRARY "OPENGL32.dll"

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace native {

  // TAG: GCC - GDI is namespace due to bug in GCC (see report 4507)
  namespace GDI {
    
    HGLRC (CALL_GDI *wglCreateContext)(HDC hdc);
    HGLRC (CALL_GDI *wglCreateLayerContext)(HDC hdc, int plane);
    BOOL (CALL_GDI *wglDeleteContext)(HGLRC hglrc);
    HGLRC (CALL_GDI *wglGetCurrentContext)();
    HDC (CALL_GDI *wglGetCurrentDC)();
    PROC (CALL_GDI *wglGetProcAddress)(LPCSTR lpszProc);
    BOOL (CALL_GDI *wglMakeCurrent)(HDC hdc, HGLRC hglrc);
    BOOL (CALL_GDI *wglSwapBuffers)(HDC hdc);
    BOOL (CALL_GDI *wglSwapLayerBuffers)(HDC hdc, UINT planes);
    BOOL (CALL_GDI *wglDescribeLayerPlane)(HDC hdc, int pixelFormat, int layerPlane, UINT bytes, LPLAYERPLANEDESCRIPTOR lpd);
    
    FunctionDescriptor FUNCTIONS[] = {
      {
        "wglCreateContext",
        Cast::pointer<Function*>((void*)&wglCreateContext)
      },
      {
        "wglCreateLayerContext",
        Cast::pointer<Function*>((void*)&wglCreateLayerContext)
      },
      {
        "wglDeleteContext",
        Cast::pointer<Function*>((void*)&wglDeleteContext)
      },
      {
        "wglGetCurrentContext",
        Cast::pointer<Function*>((void*)&wglGetCurrentContext)
      },
      {
        "wglGetCurrentDC",
        Cast::pointer<Function*>((void*)&wglGetCurrentDC)
      },
      {
        "wglGetProcAddress",
        Cast::pointer<Function*>((void*)&wglGetProcAddress)
      },
      {
        "wglMakeCurrent",
        Cast::pointer<Function*>((void*)&wglMakeCurrent)
      },
      {
        "wglSwapBuffers",
        Cast::pointer<Function*>((void*)&wglSwapBuffers)
      },
      {
        "wglSwapLayerBuffers",
        Cast::pointer<Function*>((void*)&wglSwapLayerBuffers)
      },
      {
        "wglDescribeLayerPlane",
        Cast::pointer<Function*>((void*)&wglDescribeLayerPlane)
      }
    };

    DynamicLinker* load()
    {
      // TAG: only load GDI version specific function if available
      DynamicLinker* dynamicLinker = new DynamicLinker(MESSAGE(_COM_AZURE_DEV__BASE__GDI_LIBRARY));
      if (dynamicLinker) {
        for (unsigned int i = 0; i < getArraySize(FUNCTIONS); ++i) {
          *FUNCTIONS[i].function =
            (Function)dynamicLinker->getUncertainSymbol(
              NativeString(FUNCTIONS[i].symbol)
            );
          if (!*FUNCTIONS[i].function) {
            dynamicLinker = nullptr;
            return false;
          }
        }
      }
      return dynamicLinker;
    }

    bool import() noexcept
    {
      static DynamicLinker* dynamicLinker = load();
      return dynamicLinker;
    }
  }; // end of GDI namespace
  
}; // end of native namespace

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
