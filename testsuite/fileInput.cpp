/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <typeinfo>

void readFile(String<>& filename) {
  fout << "Initializing file input stream\n";
  FileInputStream file(filename, 0);

  fout << "Binding file input stream to format input stream\n";
  FormatInputStream format(file);

  fout << "getPath(): " << file.getPath() << EOL;

  while (format.available()) {
    char ch;
    format >> ch;
    fout << ch;
  }
}

int main(int argc, char* argv[]) {
  fout << "Testing implementation of the FileInputStream\n";

  if (argc != 2) {
    fout << "fileInput filename\n";
    return 0; // stop
  }
  String<> filename = argv[1];

  try {
    readFile(filename);
  } catch(Exception& e) {
    ferr << typeid(e).name() << ": " << e.getMessage() << EOL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << EOL;
    return 1;
  }

  return 0;
}
