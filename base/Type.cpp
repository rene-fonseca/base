/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Type.h>
#include <base/string/String.h>
#include <stdlib.h>

#if defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV3)
  extern "C" char* cplus_demangle_v3(const char* mangled);
#elif defined(_DK_SDU_MIP__BASE__DEMANGLE_LIBERTY)
  extern "C" char* cplus_demangle(const char* mangled, int options);
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(_DK_SDU_MIP__BASE__DEMANGLE_GCCV3)

String demangleTypename(const char* mangled) throw() {
  static const String prefix("_Z");
  String temp = prefix; // make function name
  temp.append(mangled);
  char* demangled = cplus_demangle_v3(temp.getElements());
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
