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

#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__UNIX)
#  error native GLX interface requires unix flavor
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
#if defined(__has_include)
#if __has_include(<X11/Xlib.h>) && __has_include(<X11/Xutil.h>) && __has_include(<X11/Xatom.h>)
#  include <X11/Xlib.h>
#  include <X11/Xutil.h>
#  include <X11/Xatom.h>
#  define _COM_AZURE_DEV__BASE__USE_X11
#endif
#endif
#endif // flavor

#if !defined(_COM_AZURE_DEV__BASE__USE_X11)

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

#endif // flavor
