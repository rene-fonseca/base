/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/Type.h>
#include <base/string/String.h>
#include <stdlib.h>

#if defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV3)
  extern "C" char* cplus_demangle_v3(const char* mangled);
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV23)
  extern "C" char* cplus_demangle_new_abi(const char* mangled);
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV2)
  extern "C" char* cplus_demangle(const char* mangled, int options);
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_SUNWSPRO)
  #include <demangle.h>
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_MIPSPRO)
  #include <dem.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV3)

String demangleTypename(const char* mangled) throw() {
  static const String prefix("_Z");
  String temp = prefix;
  temp.append(mangled);
  char* demangled = cplus_demangle_v3(temp.getElements());
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  return result;
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV23)

String demangleTypename(const char* mangled) throw() {
  static const String prefix("_Z");
  String temp = prefix;
  temp.append(mangled);
  char* demangled = cplus_demangle_new_abi(temp.getElements());
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  return result;
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV2)

String demangleTypename(const char* mangled) throw() {
  static const String prefix("a__");
  static const String suffix("::a");
  // cplus_demangle only demangles function names - need alternative demangler
  String temp = prefix; // make function name
  temp.append(mangled);
  char* demangled = cplus_demangle(temp.getElements(), 1<<1); // include const and volatile
  if (!demangled) { // failed?
    return String(mangled); // return mangled type name
  }
  String result(demangled);
  free(demangled);
  return result - suffix; // remove function name
}

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_SUNWSPRO)

#error Demangling not implemented

#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_MIPSPRO)

#error Demangling not implemented

#else // no demangling support

String demangleTypename(const char* mangled) throw() {
  return mangled;
}

#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
