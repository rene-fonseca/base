/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Application.h>
#include <base/Type.h>
#include <stdlib.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Application* Application::application = 0; // initialize application as uninitialized

Application::Application(int argc, const char* argv[], const char* envp[]) throw(SingletonException, OutOfDomain) {
  static unsigned int singleton = 0;
  assert(singleton++ == 0, SingletonException("Application has been instantiated"));

  assert((argc > 0) && (argv), OutOfDomain());
  path = argv[0];
  for (unsigned int i = 1; i < argc; ++i) {
    arguments.append(argv[i]);
  }

  for (; *envp != 0; ++envp) {
    String temp(*envp);
    int index = temp.indexOf('=');
    if (index != -1) { // ignore the environment string if it doesn't contain '='
      environment[temp.substring(0, index - 1)] = temp.substring(index + 1);
    }
  }

  application = this;
}

int Application::exceptionHandler(Exception& e) const throw() {
  ferr << getTypename(e) << ": " << e.getMessage() << ENDL;
  return 1;
}

int Application::exceptionHandler() const throw() {
  ferr << "Unknown exception" << ENDL;
  return 1;
}
/*
int main(int argc, const char* argv[], const char* envp[]) throw() {
  Application app(argc, argv, envp);
  try {
  } catch(Exception& e) {
    return app.exceptionHandler(e);
  } catch(...) {
    return app.exceptionHandler();
  }
  return 0;
}
*/
_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
