/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/dl/DynamicLinker.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <dlfcn.h>
  // #include <link.h> // TAG: is this required for Solaris
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void* DynamicLinker::getGlobalSymbolImpl(const String& symbol) throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  void* result = (void*)(GetProcAddress(GetModuleHandle(0), symbol.getElements())); // TAG: possible resource leak? - should I handle possible error
  assert(result != 0, LinkerException("Unable to resolve symbol")); // (GetLastError() != ERROR_SUCCESS)
  return result;
#else // unix
  void* handle = dlopen(0, RTLD_LAZY);
  if (handle == 0) {
    ferr << MESSAGE("Linker error: ") << dlerror() << ENDL;
    throw LinkerException("Unable to open module - this should NEVER fail");
  }
  void* result = dlsym(handle, symbol.getElements());
  assert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  dlclose(handle); // TAG: I do not expect any error
  return result;
#endif // flavour
}

DynamicLinker::DynamicLinker(const String& module, unsigned int options) throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if ((handle = LoadLibraryEx(module.getElements(), 0, 0)) == 0) {
    throw LinkerException("Unable to open module");
  }
#else // unix
  int flags = (options & LAZY) ? RTLD_LAZY : RTLD_NOW;
  flags |= (options & GLOBAL) ? RTLD_GLOBAL : RTLD_LOCAL;
  if ((handle = dlopen(module.getElements(), flags)) == 0) {
    ferr << MESSAGE("Linker error: ") << dlerror() << ENDL;
    throw LinkerException("Unable to open module");
  }
#endif // flavour
}

void* DynamicLinker::getSymbol(const StringLiteral& symbol) const throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  void* result = (void*)(GetProcAddress((HMODULE)handle, symbol));
  assert(result != 0, LinkerException("Unable to resolve symbol")); // (GetLastError() != ERROR_SUCCESS)
  return result;
#else // unix
  void* result = dlsym(handle, symbol);
  assert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  return result;
#endif // flavour
}

void* DynamicLinker::getSymbol(const String& symbol) const throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  void* result = (void*)(GetProcAddress((HMODULE)handle, symbol.getElements()));
  assert(result != 0, LinkerException("Unable to resolve symbol")); // (GetLastError() != ERROR_SUCCESS)
  return result;
#else // unix
  void* result = dlsym(handle, symbol.getElements());
  assert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  return result;
#endif // flavour
}

DynamicLinker::~DynamicLinker() throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  assert(FreeLibrary((HMODULE)handle), LinkerException("Unable to close module"));
#else // unix
  assert(dlclose(handle) == 0, LinkerException("Unable to close module"));
#endif // flavour
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
