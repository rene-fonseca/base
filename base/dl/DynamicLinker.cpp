/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/dl/DynamicLinker.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <dlfcn.h>
  // #include <link.h> // TAG: is this required for Solaris
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void* DynamicLinker::getGlobalSymbolImpl(const String& symbol) throw(LinkerException) {
#if defined(__win32__)
  void* result = GetProcAddress(GetModuleHandle(), symbol.getElements()); // TAG: possible resource leak? - should I handle possible error
  assert(result != 0, LinkerException("Unable to resolve symbol")); // (GetLastError() != ERROR_SUCCESS)
  return result;
#else // __unix__
  void* handle = dlopen(0, RTLD_LAZY);
  if (handle == 0) {
    ferr << MESSAGE("Linker error: ") << dlerror() << ENDL;
    throw LinkerException("Unable to open module - this should NEVER fail");
  }
  void* result = dlsym(handle, symbol.getElements());
  assert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  dlclose(handle); // TAG: I do not expect any error
  return result;
#endif
}

DynamicLinker::DynamicLinker(const String& module, unsigned int options) throw(LinkerException) {
#if defined(__win32__)
  if ((handle = LoadLibraryEx(module.getElements(), 0, 0)) == 0) {
    throw LinkerException("Unable to open module");
  }
#else // __unix__
  int flags = (options & LAZY) ? RTLD_LAZY : RTLD_NOW;
  flags |= (options & GLOBAL) ? RTLD_GLOBAL : RTLD_LOCAL;
  if ((handle = dlopen(module.getElements(), flags)) == 0) {
    ferr << MESSAGE("Linker error: ") << dlerror() << ENDL;
    throw LinkerException("Unable to open module");
  }
#endif
}

void* DynamicLinker::getSymbol(const StringLiteral& symbol) const throw(LinkerException) {
#if defined(__win32__)
  void* result = GetProcAddress(handle, symbol.getElements());
  assert(result != 0, LinkerException("Unable to resolve symbol")); // (GetLastError() != ERROR_SUCCESS)
  return result;
#else // __unix__
  void* result = dlsym(handle, symbol);
  assert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  return result;
#endif
}

void* DynamicLinker::getSymbol(const String& symbol) const throw(LinkerException) {
#if defined(__win32__)
  void* result = GetProcAddress(handle, symbol.getElements());
  assert(result != 0, LinkerException("Unable to resolve symbol")); // (GetLastError() != ERROR_SUCCESS)
  return result;
#else // __unix__
  void* result = dlsym(handle, symbol.getElements());
  assert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  return result;
#endif
}

DynamicLinker::~DynamicLinker() throw(LinkerException) {
#if defined(__win32__)
  assert(FreeLibrary(handle), LinkerException("Unable to close module"));
#else // __unix__
  assert(dlclose(handle) == 0, LinkerException("Unable to close module"));
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
