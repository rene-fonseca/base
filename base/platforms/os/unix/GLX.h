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

#if (_DK_SDU_MIP__BASE__FLAVOR != _DK_SDU_MIP__BASE__UNIX)
#  error native GLX interface requires unix flavor
#endif

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__MACOS)

// fake types

typedef void* XID;
typedef unsigned int XVisualInfo;
typedef unsigned int Display;
typedef unsigned int Bool;
typedef void* Pixmap;
typedef void* Window;
typedef unsigned int Screen;
typedef unsigned int Font;

enum {
  None = 0,
  Success = 1,
  False = 0,
  True = 1,

  StructureNotifyMask = 0x1000,
  KeymapStateMask,
  FocusChangeMask,
  ButtonReleaseMask,
  ButtonPressMask,
  KeyReleaseMask,
  KeyPressMask,
  VisibilityChangeMask,
  EnterWindowMask,
  LeaveWindowMask,
  PointerMotionMask
};

inline void XDestroyWindow(Display*, Window) {}
inline int XDefaultScreen(Display*) {return 0;}
inline void* XScreenOfDisplay(Display*, int screenId) {return 0;}
inline int XBlackPixelOfScreen(Screen*) {return 0;}
inline int XWhitePixelOfScreen(Screen*) {return 0;}
inline void* DefaultRootWindow(Display*) {return 0;}
inline void XFree(XVisualInfo*) {}

inline void* XCreateSimpleWindow(
    Display*,
    void*,
    int,
    int,
    int,
    int,
    int, // border width
    int, // border color
    int) {return 0;}

inline void XSelectInput(Display*, Window, int) {}
inline int XScreenNumberOfScreen(Screen*) {return 0;}
inline void* XCreatePixmap(Display*, void*, int, int, int) {return 0;}
inline void XFreePixmap(Display*, Pixmap) {}

#elif (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
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

    enum {
      VERSION_1_1 = 1,
      VERSION_1_2 = 1,
      VERSION_1_3 = 1,
      
      USE_GL = 1,
      BUFFER_SIZE = 2,
      LEVEL = 3,
      RGBA = 4,
      DOUBLEBUFFER = 5,
      STEREO = 6,
      AUX_BUFFERS = 7,
      RED_SIZE = 8,
      GREEN_SIZE = 9,
      BLUE_SIZE = 10,
      ALPHA_SIZE = 11,
      DEPTH_SIZE = 12,
      STENCIL_SIZE = 13,
      ACCUM_RED_SIZE = 14,
      ACCUM_GREEN_SIZE = 15,
      ACCUM_BLUE_SIZE = 16,
      ACCUM_ALPHA_SIZE = 17,

      X_VISUAL_TYPE = 0x22,
      CONFIG_CAVEAT = 0x20,
      TRANSPARENT_TYPE = 0x23,
      TRANSPARENT_INDEX_VALUE = 0x24,
      TRANSPARENT_RED_VALUE = 0x25,
      TRANSPARENT_GREEN_VALUE = 0x26,
      TRANSPARENT_BLUE_VALUE = 0x27,
      TRANSPARENT_ALPHA_VALUE = 0x28,
      DRAWABLE_TYPE = 0x8010,
      RENDER_TYPE = 0x8011,
      RENDERABLE = 0x8012,
      FBCONFIG_ID = 0x8013,
      MAX_PBUFFER_WIDTH = 0x8016,
      MAX_PBUFFER_HEIGHT = 0x8017,
      MAX_PBUFFER_PIXELS = 0x8018,
      VISUAL_ID = 0x800b,

      DONT_CARE = 0xffffffff,
      
      BAD_SCREEN = 1,
      BAD_ATTRIBUTE = 2,
      NO_EXTENSION = 3,
      BAD_VISUAL = 4,
      BAD_CONTEXT = 5,
      BAD_VALUE = 6,
      BAD_ENUM = 7,
      
      RGBA_BIT = 0x00000001, // CONFIG_CAVEAT attribute
      COLOR_INDEX_BIT = 0x00000002, // CONFIG_CAVEAT attribute
      
      WINDOW_BIT = 0x00000001, // X_VISUAL_TYPE attribute
      PIXMAP_BIT = 0x00000002, // X_VISUAL_TYPE attribute
      PBUFFER_BIT = 0x00000004, // X_VISUAL_TYPE attribute
      
      NONE = 0x8000, // CONFIG_CAVEAT attribute
      SLOW_CONFIG = 0x8001, // CONFIG_CAVEAT attribute
      NON_CONFORMANT_CONFIG = 0x800d, // CONFIG_CAVEAT attribute
      
      TRUE_COLOR = 0x8002, // X_VISUAL_TYPE attribute
      DIRECT_COLOR = 0x8003, // X_VISUAL_TYPE attribute
      PSEUDO_COLOR = 0x8004, // X_VISUAL_TYPE attribute
      STATIC_COLOR = 0x8005, // X_VISUAL_TYPE attribute
      GRAY_SCALE = 0x8006, // X_VISUAL_TYPE attribute
      STATIC_GRAY = 0x8007, // X_VISUAL_TYPE attribute
      
      TRANSPARENT_RGB = 0x8008,
      TRANSPARENT_INDEX = 0x8009,      

      PRESERVED_CONTENTS = 0x801b,
      LARGEST_PBUFFER = 0x801c,
      PBUFFER_HEIGHT = 0x8040, // GLX 1.3
      PBUFFER_WIDTH = 0x8041, // GLX 1.3
      
      WIDTH = 0x801d,
      HEIGHT = 0x801e,
      EVENT_MASK = 0x801f,
      
      RGBA_TYPE = 0x8014,
      COLOR_INDEX_TYPE = 0x8015,

      SCREEN = 0x800c,
      
      PBUFFER_CLOBBER_MASK = 0x08000000,

      DAMAGED = 0x8020,
      SAVED = 0x8021,
      
      WINDOW = 0x8022,
      PBUFFER = 0x8023,

      FRONT_LEFT_BUFFER_BIT = 0x00000001,
      FRONT_RIGHT_BUFFER_BIT = 0x00000002,
      BACK_LEFT_BUFFER_BIT = 0x00000004,
      BACK_RIGHT_BUFFER_BIT = 0x00000008,
      AUX_BUFFERS_BIT = 0x00000010,
      DEPTH_BUFFER_BIT = 0x00000020,
      STENCIL_BUFFER_BIT = 0x00000040,
      ACCUM_BUFFER_BIT = 0x00000080,

      VENDOR = 0x1,
      VERSION = 0x2,
      EXTENSIONS = 0x3
    };
    
    static int errorBase;
    static int eventBase;
    static unsigned int version;
    
    typedef void (CALL_GLX *Function)();
    
    struct FunctionDescriptor {
      const char* symbol;
      Function* function;
    };

    typedef unsigned int GLenum;
    typedef unsigned char GLboolean;
    typedef unsigned int GLbitfield;
    typedef signed char GLbyte;
    typedef short GLshort;
    typedef int GLint;
    typedef int GLsizei;
    typedef unsigned char GLubyte;
    typedef unsigned short GLushort;
    typedef unsigned int GLuint;
    typedef float GLfloat;
    typedef float GLclampf;
    typedef double GLdouble;
    typedef double GLclampd;
    
    typedef XID GLXContextID;
    typedef XID GLXPixmap;
    typedef XID GLXDrawable;
    typedef XID GLXPbuffer;
    typedef XID GLXWindow;
    typedef XID GLXFBConfigID;
    
    typedef struct {}* GLXContext;
    typedef struct {}* GLXFBConfig;
    
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
