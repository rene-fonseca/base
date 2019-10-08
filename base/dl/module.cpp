/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

extern void moduleEntry();
extern void moduleCleanUp();

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)

#include <windows.h>

extern "C" BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    base::moduleEntry();
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    base::moduleCleanUp();
  }
  return TRUE;
}

#else // unix

extern "C" void _init() {
  base::moduleEntry();
}

extern "C" void _fini() {
  base::moduleCleanUp();
}

#endif // flavor
