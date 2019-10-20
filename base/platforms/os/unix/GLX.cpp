/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#if defined(_COM_AZURE_DEV__BASE__ALLOW_GLX)

#include <base/platforms/features.h>
#include <base/platforms/os/unix/GLX.h>
#include <base/dl/DynamicLinker.h>
#include <base/platforms/backend/WindowImpl.h>

// TAG: put in static description
#define _COM_AZURE_DEV__BASE__GLX_LIBRARY "libGL.so"

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace native {
  
  int GLX::errorBase = 0;
  int GLX::eventBase = 0;
  unsigned int GLX::version = 0;
  
  // GLX 1.0
  XVisualInfo* (CALL_GLX *GLX::glXChooseVisual)(Display* dpy, int screen, int* attribList);
  void (CALL_GLX *GLX::glXCopyContext)(Display* dpy, GLX::GLXContext src, GLX::GLXContext dst, GLuint mask);
  GLX::GLXContext (CALL_GLX *GLX::glXCreateContext)(Display* dpy, XVisualInfo* vis, GLX::GLXContext shareList, Bool direct);
  GLX::GLXPixmap (CALL_GLX *GLX::glXCreateGLXPixmap)(Display* dpy, XVisualInfo* vis, Pixmap pixmap);
  void (CALL_GLX *GLX::glXDestroyContext)(Display* dpy, GLX::GLXContext ctx);
  void (CALL_GLX *GLX::glXDestroyGLXPixmap)(Display* dpy, GLX::GLXPixmap pix);
  int (CALL_GLX *GLX::glXGetConfig)(Display* dpy, XVisualInfo* vis, int attrib, int* value);
  GLX::GLXContext (CALL_GLX *GLX::glXGetCurrentContext)();
  GLX::GLXDrawable (CALL_GLX *GLX::glXGetCurrentDrawable)();
  Bool (CALL_GLX *GLX::glXIsDirect)(Display* dpy, GLX::GLXContext ctx);
  Bool (CALL_GLX *GLX::glXMakeCurrent)(Display* dpy, GLX::GLXDrawable drawable, GLX::GLXContext ctx);
  Bool (CALL_GLX *GLX::glXQueryExtension)(Display* dpy, int* errorBase, int* eventBase);
  Bool (CALL_GLX *GLX::glXQueryVersion)(Display* dpy, int* major, int* minor);
  void (CALL_GLX *GLX::glXSwapBuffers)(Display* dpy, GLX::GLXDrawable drawable);
  void (CALL_GLX *GLX::glXUseXFont)(Font font, int first, int count, int listBase);
  void (CALL_GLX *GLX::glXWaitGL)();
  void (CALL_GLX *GLX::glXWaitX)();
  
  // GLX 1.1
  const char* (CALL_GLX *GLX::glXQueryExtensionsString)(Display* dpy, int screen);
  const char* (CALL_GLX *GLX::glXGetClientString)(Display* dpy, int name);
  const char* (CALL_GLX *GLX::glXQueryServerString)(Display* dpy, int screen, int name);
  
  // GLX 1.3
  GLX::GLXFBConfig* (CALL_GLX *GLX::glXGetFBConfigs)(Display* dpy, int screen, int* nelements);
  GLX::GLXFBConfig* (CALL_GLX *GLX::glXChooseFBConfig)(Display* dpy, int screen, const int* attrib_list, int* nelements);
  int (CALL_GLX *GLX::glXGetFBConfigAttrib)(Display* dpy, GLX::GLXFBConfig config, int attribute, int* value);
  XVisualInfo* (CALL_GLX *GLX::glXGetVisualFromFBConfig)(Display* dpy, GLX::GLXFBConfig config);
  GLX::GLXWindow (CALL_GLX *GLX::glXCreateWindow)(Display* dpy, GLX::GLXFBConfig config, Window win, const int* attrib_list);
  void (CALL_GLX *GLX::glXDestroyWindow)(Display* dpy, GLX::GLXWindow win);
  GLX::GLXPixmap (CALL_GLX *GLX::glXCreatePixmap)(Display* dpy, GLX::GLXFBConfig config, Pixmap pixmap, const int* attrib_list);
  void (CALL_GLX *GLX::glXDestroyPixmap)(Display* dpy, GLX::GLXPixmap pixmap);
  GLX::GLXPbuffer (CALL_GLX *GLX::glXCreatePbuffer)(Display* dpy, GLX::GLXFBConfig config, const int* attrib_list);
  void (CALL_GLX *GLX::glXDestroyPbuffer)(Display* dpy, GLX::GLXPbuffer pbuf);
  void (CALL_GLX *GLX::glXQueryDrawable)(Display* dpy, GLX::GLXDrawable draw, int attribute, unsigned int* value);
  GLX::GLXContext (CALL_GLX *GLX::glXCreateNewContext)(Display* dpy, GLX::GLXFBConfig config, int render_type, GLX::GLXContext share_list, Bool direct);
  Bool (CALL_GLX *GLX::glXMakeContextCurrent)(Display* display, GLX::GLXDrawable draw, GLX::GLXDrawable read, GLX::GLXContext ctx);
  GLX::GLXDrawable (CALL_GLX *GLX::glXGetCurrentReadDrawable)();
  Display* (CALL_GLX *GLX::glXGetCurrentDisplay)();
  int (CALL_GLX *GLX::glXQueryContext)(Display* dpy, GLX::GLXContext ctx, int attribute, int* value);
  void (CALL_GLX *GLX::glXSelectEvent)(Display* dpy, GLX::GLXDrawable draw, unsigned long event_mask);
  void (CALL_GLX *GLX::glXGetSelectedEvent)(Display* dpy, GLX::GLXDrawable draw, unsigned long* event_mask);

  // supports up to OpenGL 1.0
  GLX::FunctionDescriptor GLX_FUNCTIONS_1_0[] = {
    // GLX 1.0
    {"glXChooseVisual", Cast::pointer<GLX::Function*>((void*)&GLX::glXChooseVisual)},
    {"glXCopyContext", Cast::pointer<GLX::Function*>((void*)&GLX::glXCopyContext)},
    {"glXCreateContext", Cast::pointer<GLX::Function*>((void*)&GLX::glXCreateContext)},
    {"glXCreateGLXPixmap", Cast::pointer<GLX::Function*>((void*)&GLX::glXCreateGLXPixmap)},
    {"glXDestroyContext", Cast::pointer<GLX::Function*>((void*)&GLX::glXDestroyContext)},
    {"glXDestroyGLXPixmap", Cast::pointer<GLX::Function*>((void*)&GLX::glXDestroyGLXPixmap)},
    {"glXGetConfig", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetConfig)},
    {"glXGetCurrentContext", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetCurrentContext)},
    {"glXGetCurrentDrawable", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetCurrentDrawable)},
    {"glXIsDirect", Cast::pointer<GLX::Function*>((void*)&GLX::glXIsDirect)},
    {"glXMakeCurrent", Cast::pointer<GLX::Function*>((void*)&GLX::glXMakeCurrent)},
    {"glXQueryExtension", Cast::pointer<GLX::Function*>((void*)&GLX::glXQueryExtension)},
    {"glXQueryVersion", Cast::pointer<GLX::Function*>((void*)&GLX::glXQueryVersion)},
    {"glXSwapBuffers", Cast::pointer<GLX::Function*>((void*)&GLX::glXSwapBuffers)},
    {"glXUseXFont", Cast::pointer<GLX::Function*>((void*)&GLX::glXUseXFont)},
    {"glXWaitGL", Cast::pointer<GLX::Function*>((void*)&GLX::glXWaitGL)},
    {"glXWaitX", Cast::pointer<GLX::Function*>((void*)&GLX::glXWaitX)}
  };
  
  // supports up to OpenGL 1.0
  // GLX 1.1
  GLX::FunctionDescriptor GLX_FUNCTIONS_1_1[] = {
    {"glXQueryExtensionsString", Cast::pointer<GLX::Function*>((void*)&GLX::glXQueryExtensionsString)},
    {"glXGetClientString", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetClientString)},
    {"glXQueryServerString", Cast::pointer<GLX::Function*>((void*)&GLX::glXQueryServerString)}
  };
  
  // supports up to OpenGL 1.1
  // GLX 1.2
  GLX::FunctionDescriptor GLX_FUNCTIONS_1_2[] = {
    {"glXGetCurrentDisplay", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetCurrentDisplay)}
  };
  
  // supports up to OpenGL 1.2
  // GLX 1.3
  GLX::FunctionDescriptor GLX_FUNCTIONS_1_3[] = {
    {"glXGetFBConfigs", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetFBConfigs)},
    {"glXChooseFBConfig", Cast::pointer<GLX::Function*>((void*)&GLX::glXChooseFBConfig)},
    {"glXGetFBConfigAttrib", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetFBConfigAttrib)},
    {"glXGetVisualFromFBConfig", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetVisualFromFBConfig)},
    {"glXCreateWindow", Cast::pointer<GLX::Function*>((void*)&GLX::glXCreateWindow)},
    {"glXDestroyWindow", Cast::pointer<GLX::Function*>((void*)&GLX::glXDestroyWindow)},
    {"glXCreatePixmap", Cast::pointer<GLX::Function*>((void*)&GLX::glXCreatePixmap)},
    {"glXDestroyPixmap", Cast::pointer<GLX::Function*>((void*)&GLX::glXDestroyPixmap)},
    {"glXCreatePbuffer", Cast::pointer<GLX::Function*>((void*)&GLX::glXCreatePbuffer)},
    {"glXDestroyPbuffer", Cast::pointer<GLX::Function*>((void*)&GLX::glXDestroyPbuffer)},
    {"glXQueryDrawable", Cast::pointer<GLX::Function*>((void*)&GLX::glXQueryDrawable)},
    {"glXCreateNewContext", Cast::pointer<GLX::Function*>((void*)&GLX::glXCreateNewContext)},
    {"glXMakeContextCurrent", Cast::pointer<GLX::Function*>((void*)&GLX::glXMakeContextCurrent)},
    {"glXGetCurrentReadDrawable", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetCurrentReadDrawable)},
    {"glXQueryContext", Cast::pointer<GLX::Function*>((void*)&GLX::glXQueryContext)},
    {"glXSelectEvent", Cast::pointer<GLX::Function*>((void*)&GLX::glXSelectEvent)},
    {"glXGetSelectedEvent", Cast::pointer<GLX::Function*>((void*)&GLX::glXGetSelectedEvent)}
  };

  bool GLX::import() throw() {
    // TAG: only load GLX version specific function if available
    static DynamicLinker* dynamicLinker = 0;
    if (!dynamicLinker) {
      // never release
      dynamicLinker = new DynamicLinker(
        MESSAGE(_COM_AZURE_DEV__BASE__GLX_LIBRARY)
      );
      if (!dynamicLinker) {
        return false;
      }
      for (unsigned int i = 0; i < getArraySize(GLX_FUNCTIONS_1_0); ++i) {
        *GLX_FUNCTIONS_1_0[i].function =
          (Function)dynamicLinker->getUncertainSymbol(
            NativeString(GLX_FUNCTIONS_1_0[i].symbol)
          );
        if (!*GLX_FUNCTIONS_1_0[i].function) {
          dynamicLinker = 0;
          return false;
        }
      }

      Bool result = native::GLX::glXQueryExtension(
        (Display*)Backend<WindowImpl>::getDisplay(),
        &errorBase,
        &eventBase
      );
      if (result != True) {
        return false;
      }
      
      int major;
      int minor;
      result = native::GLX::glXQueryVersion(
        (Display*)Backend<WindowImpl>::getDisplay(),
        &major,
        &minor
      );
      version = minimum(major, 255) << 16 | minimum(minor, 255) << 8;

      if (version >= 0x010100) {
        for (unsigned int i = 0; i < getArraySize(GLX_FUNCTIONS_1_1); ++i) {
          *GLX_FUNCTIONS_1_1[i].function =
            (Function)dynamicLinker->getUncertainSymbol(
              NativeString(GLX_FUNCTIONS_1_1[i].symbol)
            );
          if (!*GLX_FUNCTIONS_1_1[i].function) {
            dynamicLinker = 0;
            return false;
          }
        }
        if (version >= 0x010300) {
          for (unsigned int i = 0; i < getArraySize(GLX_FUNCTIONS_1_3); ++i) {
            *GLX_FUNCTIONS_1_3[i].function =
              (Function)dynamicLinker->getUncertainSymbol(
                NativeString(GLX_FUNCTIONS_1_3[i].symbol)
              );
            if (!*GLX_FUNCTIONS_1_3[i].function) {
              dynamicLinker = 0;
              return false;
            }
          }
        }
      }
    }
    return true;
  }

}; // end of native namespace

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

#endif // _COM_AZURE_DEV__BASE__ALLOW_GLX
