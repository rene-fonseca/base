/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_BACKEND__WINDOW_IMPL_H
#define _DK_SDU_MIP__BASE_PLATFORMS_BACKEND__WINDOW_IMPL_H

#include <base/ui/WindowImpl.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  undef DELETE // yikes
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define CALL_UI _DK_SDU_MIP__BASE__CALL_PASCAL
#else
#  define CALL_UI
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Backend<WindowImpl> {
public:

  static inline void destroy(WindowImpl* windowImpl) throw() {
    windowImpl->destroy();
  }
  
  static inline void* getDisplay() throw() {
    return WindowImpl::displayHandle;
  }
  
  static inline bool loadModule(bool load) throw() {
    return WindowImpl::loadModule(load);
  }

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  static LONG CALL_UI messageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) throw();
#endif // flavor
  
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
