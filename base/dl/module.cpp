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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

extern void moduleEntry();
extern void moduleCleanUp();

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

#include <windows.h>

extern "C" BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH) {
    base::moduleEntry();
  } else if (fdwReason == DLL_PROCESS_DETACH) {
    base::moduleCleanUp();
  }
  return TRUE;
}

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)

void __attribute__ ((constructor)) moduleInit()
{
  base::moduleEntry();
}

void __attribute__ ((destructor)) moduleFini()
{
  base::moduleCleanUp();
}

#else // unix

#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREETROS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__SOLARIS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREEBSD) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__OPENBSD)
extern "C" void _init()
{
  base::moduleEntry();
}

extern "C" void _fini()
{
  base::moduleCleanUp();
}
#endif

#endif // flavor
