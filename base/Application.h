/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__APPLICATION_H
#define _DK_SDU_MIP__BASE__APPLICATION_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/collection/Map.h>
#include <base/OutOfDomain.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This is a singleton object for the context of the application.

  @short Application.
  @author Ren� M�ller Fonseca
*/

class Application : public Object {
private:

  /** The path used to start the application. */
  String path;
  /** Container for the arguments passed to the application. */
  Array<String> arguments;
  /** Container for the environment variables. */
  Map<String, String> environment;
public:

  /**
    Initializes application.

    <pre>
    int main(int argc, const char* argv[], const char* envp[]) {
      Application app(argc, argv, envp);
      ...
    }
    </pre>

    @param argc The argc argument of the entry function main.
    @param argv The argv argument of the entry function main.
    @param envp The envp argument of the entry function main. This argument is not required.
  */
  Application(int argc, const char* argv[], const char *envp[] = 0) throw(OutOfDomain);

  /**
    Returns the name/path of the application.
  */
  inline String getName() const throw() {return path;}

  /**
    Returns the arguments passed to the application.
  */
  inline const Array<String>& getArguments() const throw() {return arguments;}

  /**
    Returns the environment variables.
  */
  inline const Map<String, String> getEnvironment() const throw() {return environment;}

  /**
    Destroys the application.
  */
  ~Application() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
