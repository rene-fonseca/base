/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_OS_UNIX__GLX_H
#define _DK_SDU_MIP__BASE_PLATFORMS_OS_UNIX__GLX_H

#include <base/platforms/features.h>

#if (_DK_SDU_MIP__BASE__FLAVOR != _DK_SDU_MIP__BASE__UNIX)
#  error native GLX interface requires unix flavor
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
#  include <GL/glx.h>
#endif // flavor

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_GLX _DK_SDU_MIP__BASE__CALL_PASCAL
#else
#  define CALL_GLX
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace native {

  class GLX {
  public:

    static int errorBase;
    static int eventBase;
    static unsigned int version;
    
    typedef void (CALL_GLX *Function)();
    
    struct FunctionDescriptor {
      const char* symbol;
      Function* function;
    };
    
    // GLX 1.0
    static XVisualInfo* (CALL_GLX *glXChooseVisual)(Display* dpy, int screen, int* attribList);
    static void (CALL_GLX *glXCopyContext)(Display* dpy, GLXContext src, GLXContext dst, GLuint mask);
    static GLXContext (CALL_GLX *glXCreateContext)(Display* dpy, XVisualInfo* vis, GLXContext shareList, Bool direct);
    static GLXPixmap (CALL_GLX *glXCreateGLXPixmap)(Display* dpy, XVisualInfo* vis, Pixmap pixmap);
    static void (CALL_GLX *glXDestroyContext)(Display* dpy, GLXContext ctx);
    static void (CALL_GLX *glXDestroyGLXPixmap)(Display* dpy, GLXPixmap pix);
    static int (CALL_GLX *glXGetConfig)(Display* dpy, XVisualInfo* vis, int attrib, int* value);
    static GLXContext (CALL_GLX *glXGetCurrentContext)();
    static GLXDrawable (CALL_GLX *glXGetCurrentDrawable)();
    static Bool (CALL_GLX *glXIsDirect)(Display* dpy, GLXContext ctx);
    static Bool (CALL_GLX *glXMakeCurrent)(Display* dpy, GLXDrawable drawable, GLXContext ctx);
    static Bool (CALL_GLX *glXQueryExtension)(Display* dpy, int* errorBase, int* eventBase);
    static Bool (CALL_GLX *glXQueryVersion)(Display* dpy, int* major, int* minor);
    static void (CALL_GLX *glXSwapBuffers)(Display* dpy, GLXDrawable drawable);
    static void (CALL_GLX *glXUseXFont)(Font font, int first, int count, int listBase);
    static void (CALL_GLX *glXWaitGL)();
    static void (CALL_GLX *glXWaitX)();
    
    // GLX 1.1
    static const char* (CALL_GLX *glXQueryExtensionsString)(Display* dpy, int screen);
    static const char* (CALL_GLX *glXGetClientString)(Display* dpy, int name);
    static const char* (CALL_GLX *glXQueryServerString)(Display* dpy, int screen, int name);
    
    // GLX 1.3
    static GLXFBConfig* (CALL_GLX *glXGetFBConfigs)(Display* dpy, int screen, int* nelements);
    static GLXFBConfig* (CALL_GLX *glXChooseFBConfig)(Display* dpy, int screen, const int* attrib_list, int* nelements);
    static int (CALL_GLX *glXGetFBConfigAttrib)(Display* dpy, GLXFBConfig config, int attribute, int* value);
    static XVisualInfo* (CALL_GLX *glXGetVisualFromFBConfig)(Display* dpy, GLXFBConfig config);
    static GLXWindow (CALL_GLX *glXCreateWindow)(Display* dpy, GLXFBConfig config, Window win, const int* attrib_list);
    static void (CALL_GLX *glXDestroyWindow)(Display* dpy, GLXWindow win);
    static GLXPixmap (CALL_GLX *glXCreatePixmap)(Display* dpy, GLXFBConfig config, Pixmap pixmap, const int* attrib_list);
    static void (CALL_GLX *glXDestroyPixmap)(Display* dpy, GLXPixmap pixmap);
    static GLXPbuffer (CALL_GLX *glXCreatePbuffer)(Display* dpy, GLXFBConfig config, const int* attrib_list);
    static void (CALL_GLX *glXDestroyPbuffer)(Display* dpy, GLXPbuffer pbuf);
    static void (CALL_GLX *glXQueryDrawable)(Display* dpy, GLXDrawable draw, int attribute, unsigned int* value);
    static GLXContext (CALL_GLX *glXCreateNewContext)(Display* dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
    static Bool (CALL_GLX *glXMakeContextCurrent)(Display* display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
    static GLXDrawable (CALL_GLX *glXGetCurrentReadDrawable)();
    static Display* (CALL_GLX *glXGetCurrentDisplay)();
    static int (CALL_GLX *glXQueryContext)(Display* dpy, GLXContext ctx, int attribute, int* value);
    static void (CALL_GLX *glXSelectEvent)(Display* dpy, GLXDrawable draw, unsigned long event_mask);
    static void (CALL_GLX *glXGetSelectedEvent)(Display* dpy, GLXDrawable draw, unsigned long* event_mask);

    static bool import() throw();
  };
  
}; // end of native namespace

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
