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

#include <base/ui/WindowImpl.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if !defined(_WIN32_WINNT)
#  define _WIN32_WINNT _WIN32_WINNT_WINXP
#endif
#  define OEMRESOURCE
#  include <windows.h>
#  undef DELETE // yikes
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define CALL_UI __stdcall
#else
#  define CALL_UI
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template<>
class Backend<WindowImpl> {
public:

  static inline void destroy(WindowImpl* windowImpl) noexcept {
    windowImpl->destroy();
  }
  
  static inline void* getDisplay() noexcept {
    return WindowImpl::displayHandle;
  }
  
  static inline bool loadModule(bool load) noexcept {
    return WindowImpl::loadModule(load);
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  static LRESULT CALLBACK messageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
#endif // flavor
  
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
