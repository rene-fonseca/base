/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/Type.h>

using namespace base;

void readFile(const String& filename) {
  fout << "Initializing file input stream" << ENDL;
  FileInputStream file(filename, 0);

  fout << "Binding file input stream to format input stream" << ENDL;
  FormatInputStream format(file);

  while (format.available()) {
    char ch;
    format >> ch;
    fout << ch;
  }
}

int entry() {
  fout << "Testing implementation of the FileInputStream" << ENDL;

  const Array<String> arguments = Application::getApplication()->getArguments();
  if (arguments.getSize() != 1) {
    fout << Application::getApplication()->getName() << " filename" << ENDL;
    return 0; // stop
  }

  readFile(arguments[0]);
  return 0;
}

int main(int argc, const char* argv[], const char *envp[]) {
  Application app("fileInput", argc, argv, envp);
  try {
    return entry();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
}
