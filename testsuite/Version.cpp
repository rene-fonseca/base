/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/Version.h>

using namespace base;

class VersionApplication : public Application {
public:

  VersionApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("Version"), numberOfArguments, arguments, environment) {
  }

  void main() throw() {
    fout << Application::getFormalName() << MESSAGE(" version 1.0") << EOL
         << MESSAGE("Copyright (c) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << EOL
         << Version().getBanner() << ENDL;
  }

};

int main(int argc, const char* argv[], const char* envp[]) {
  VersionApplication application(argc, argv, envp);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::EXIT_CODE_NORMAL;
}
