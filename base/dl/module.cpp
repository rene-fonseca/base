/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>

extern void moduleEntry();
extern void moduleCleanUp();

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)

#include <windows.h>

extern "C" BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  if (fdwReason == DLL_PROCESS_ATTACH) {
    moduleEntry();
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    moduleCleanUp();
  }
}

#else // unix

extern "C" void _init() {
  moduleEntry();
}

extern "C" void _fini() {
  moduleCleanUp();
}

#endif // flavour
