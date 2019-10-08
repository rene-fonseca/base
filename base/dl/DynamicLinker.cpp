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
#include <base/dl/DynamicLinker.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <dlfcn.h>
// #include <link.h> // TAG: is this required for Solaris
#  if !defined(RDLD_LAZY) || !defined(RDLD_NOW) || !defined(RTLD_GLOBAL) || !defined(RTDL_LOCAL)
#    warning dlfcn.h is not compliant with IEEE Std 1003.1-2001
#  endif
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void* DynamicLinker::getGlobalSymbolImpl(
  const String& symbol) throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  // GetModuleHandle does not increment reference count
  void* result = (void*)(::GetProcAddress(::GetModuleHandle(0), symbol.getElements()));
  bassert(result != 0, LinkerException("Unable to resolve symbol"));
  return result;
#else // unix
  #if defined(RTLD_LAZY)
    void* handle = ::dlopen(0, RTLD_LAZY);
  #else
    void* handle = ::dlopen(0, 0); // use unspecified linking
  #endif
  if (handle == 0) {
    throw LinkerException("Unable to open module");
  }
  void* result = ::dlsym(handle, symbol.getElements());
  bassert(dlerror() == 0, LinkerException("Unable to resolve symbol"));
  ::dlclose(handle); // is this required
  return result;
#endif // flavor
}

// TAG: get path of module support
// TAG: enumerate modules support

//String DynamicLinker::getPath() throw() {
//#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//  String path(MAX_PATH); // maximum path length in ANSI mode
//  DWORD length = ::GetModuleFileNameEx(
//                   ::GetCurrentProcess(), // get pseudo handle
//                   ::GetModuleHandle(0), // get handle of process image
//                   path.getBytes(), // path buffer
//                   MAX_PATH
//  );
//  if (length == 0) {
//    throw LinkerException(this);
//  }
//  path.setLength(length);
//  return path;
//#else // unix
//  return String();
//#endif // flavor
//}

DynamicLinker::DynamicLinker(
  const String& path, unsigned int options) throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if ((handle = ::LoadLibraryEx(toWide(path).c_str(), 0, 0)) == nullptr) {
    throw LinkerException("Unable to open module", this);
  }
#else // unix
  #if defined(RTLD_LAZY)
    int flags = (options & LAZY) ? RTLD_LAZY : RTLD_NOW;
  #else
    int flags = RTLD_NOW; // we only end up here if dlfcn.h is non-compliant
  #endif
  #if defined(RTLD_LOCAL)
    flags |= (options & GLOBAL) ? RTLD_GLOBAL : RTLD_LOCAL;
  #else
    flags |= RTLD_GLOBAL;
  #endif
  if ((handle = ::dlopen(path.getElements(), flags)) == 0) {
    throw LinkerException("Unable to open module", this);
  }
#endif // flavor
}

void* DynamicLinker::getSymbol(
  const Literal& symbol) const throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  void* result = (void*)(::GetProcAddress((HMODULE)handle, symbol.getValue()));
  bassert(result != nullptr, LinkerException("Unable to resolve symbol", this));
  return result;
#else // unix
  void* result = ::dlsym(handle, symbol.getValue());
  bassert(dlerror() == 0, LinkerException("Unable to resolve symbol", this));
  return result;
#endif // flavor
}

void* DynamicLinker::getSymbol(
  const String& symbol) const throw(LinkerException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  void* result = (void*)(::GetProcAddress((HMODULE)handle, symbol.getElements()));
  bassert(result != 0, LinkerException("Unable to resolve symbol", this));
  return result;
#else // unix
  void* result = ::dlsym(handle, symbol.getElements());
  bassert(dlerror() == 0, LinkerException("Unable to resolve symbol", this));
  return result;
#endif // flavor
}

void* DynamicLinker::getUncertainSymbol(
  const Literal& symbol) const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return (void*)(::GetProcAddress((HMODULE)handle, symbol.getValue()));
#else // unix
  return ::dlsym(handle, symbol.getValue());
#endif // flavor
}

void* DynamicLinker::getUncertainSymbol(const String& symbol) const throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return (void*)(::GetProcAddress((HMODULE)handle, symbol.getElements()));
#else // unix
  return ::dlsym(handle, symbol.getElements());
#endif // flavor
}

bool DynamicLinker::import(
  StaticFunctionDescriptor* functions,
  unsigned int numberOfFunctions,
  bool flags) throw() {
  bool result = true;
  for (unsigned int i = 0; i < numberOfFunctions; ++i) {
    void* address = getUncertainSymbol(NativeString(functions[i].symbol));
    if (!address) {
      result = false;
      if ((flags & CONTINUE) == 0) {
        break;
      }
    }
    *functions[i].function = (Function)address;
  }
  return result;
}

DynamicLinker::~DynamicLinker() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  bassert(
    ::FreeLibrary((HMODULE)handle),
    LinkerException("Unable to close module", this)
  );
#else // unix
  bassert(
    ::dlclose(handle) == 0,
    LinkerException("Unable to close module", this)
  );
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
