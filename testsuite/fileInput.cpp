/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Type.h>

using namespace base;

void readFile(String& filename) {
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

int main(int argc, char* argv[]) {
  fout << "Testing implementation of the FileInputStream" << ENDL;

  if (argc != 2) {
    fout << "fileInput filename" << ENDL;
    return 0; // stop
  }

  String filename = argv[1];

  try {
    readFile(filename);
  } catch(Exception& e) {
    ferr << getTypename(e) << ": " << e.getMessage() << ENDL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
    return 1;
  }

  return 0;
}
