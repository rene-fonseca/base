/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Application.h>
#include <stdlib.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Application::Application(int argc, const char* argv[], const char* envp[]) throw(OutOfDomain) {
  static unsigned int singleton = 0;
  assert(singleton++ == 0, Exception("Unable to create singleton object")); // Singleton exception object
  assert((argc > 0) && (argv), OutOfDomain());
  path = argv[0];
  for (unsigned int i = 1; i < argc; ++i) {
    arguments.append(argv[i]);
  }

  while (envp && *envp) {
    String temp(*envp);
    int index = temp.indexOf('=');
//    assert(index != -1, Exception());
    if (index != -1) { // ignore string
      environment[temp.substring(0, index - 1)] = temp.substring(index + 1);
    }
    ++envp;
  }
}

Application::~Application() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
