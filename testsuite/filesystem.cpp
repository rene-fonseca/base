/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/filesystem/Folder.h>
#include <base/filesystem/File.h>
#include <base/filesystem/FileSystem.h>

using namespace base;

void test(const String& path) {
  Folder folder(path);
  Array<String> entries = folder.getEntries();

  fout << "folder: " << folder.getPath() << ENDL;
  fout << "number of entries: " << entries.getSize() << ENDL;
  fout << "entries: " << entries << ENDL;
}

int main(int argc, char* argv[]) {
  fout << "Testing filesystem..." << ENDL;

  String path = "."; // use current directory

  switch (argc) {
  case 1:
    // use defaults
    break;
  case 2:
    path = argv[1]; // the path
    break;
  default:
    fout << "filesystem [path]" << ENDL;
    return 0; // stop
  }

  try {
    test(path);
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << ENDL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
    return 1;
  }
  return 0;
}
