/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Type.h>
#include <base/string/String.h>

#if defined(_DK_SDU_MIP__BASE__DEMANGLE)
extern "C" {
  #include <demangle.h>
}
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(_DK_SDU_MIP__BASE__DEMANGLE)

String demangleTypename(const char* mangled) throw() {
//  prepends <name>__ to c_mangled and then passes result to cplus_demangle. the result from cplus_demangle is
//  <typename>::<name> where <typename> is the actual typename of
//  the class represented by c_mangled.  The code then strips the
//  ::<name> from the resulting string and returns the result.
//
//  const char* const fake_memfun = "fakeFun__";
//  String s_fake_mangled = String(fake_memfun) + String(c_mangled);
//  const char* c_fake_mangled = s_fake_mangled.getBytes();
//  int const flags = 0; // the above value should just cause function names with no args to be produced. i.e. A::f instead of, e.g. A::f(int)
//  char* c_fake_demangled = cplus_demangle(c_fake_mangled, flags);
//  assert(c_fake_demangled);
//  {
//    int const len_f = strlen(fake_memfun);
//    int const len_c = strlen(c_fake_demangled);
//    c_fake_demangled[len_c - len_f] = '\0'; //end string before ::fake_memfun
//  }
//  String s_demangled = c_fake_demangled;
//  delete c_fake_demangled;
//  return s_demangled;
  return String(mangled);
}

#else // no demangling support

String demangleTypename(const char* mangled) throw() {
  return String(mangled);
}

#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
