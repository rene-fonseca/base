/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/Type.h>
#include <base/concurrency/Thread.h>
#include <base/Daemon.h>

using namespace base;

int daemon() {
  for (unsigned int i = 0; i < 1000000000; ++i) {
  }
  return 0;
}

int entry() {
  fout << "Testing implementation of the Daemon..." << ENDL;
  if (Daemon::daemonize()) {
    return daemon();
  }
  fout << "Forked" << ENDL;
  return 0;
}

int main(int argc, const char* argv[], const char *envp[]) {
  Application app(argc, argv, envp);
  try {
    return entry();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
}
