/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FolderInfo.h>
#include <base/filesystem/FileInfo.h>
#include <base/filesystem/FileSystem.h>

using namespace base;

void test(const String& path) {
  FolderInfo folder(path);
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
