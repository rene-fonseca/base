/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/io/File.h>
#include <base/xml/XMLParser.h>
#include <base/Type.h>
#include <base/collection/Stack.h>
#include <base/collection/Array.h>

using namespace base;

class Callback : public XMLParser::Callback {
public:

  void startDocument() {
    fout << "XML - startDocument" << EOL;
  }

  void endDocument() {
    fout << "XML - endDocument" << EOL;
  }

  void startElement(const String& name, const XMLPropertyHash& attributes) {
    fout << "XML - startElement: " << name << " - {";
    XMLPropertyHash::ReadEnumerator enu = attributes.getReadEnumerator();
    while (enu.hasNext()) {
      const XMLProperty* attribute = enu.next()->getValue();
      fout << "{" << attribute->getName() << ";" << attribute->getValue() << "}";
      if (enu.hasNext()) {
        fout << ", ";
      }
    }
    fout << "}" << EOL;
  }

  void endElement(const String& n) {
    fout << "XML - endElement: " << n << EOL;
  }

  void characters(const String& s) {
    fout << "XML - characters: " << s << EOL;
  }

  void comment(const String& s) {
    fout << "XML - comment: " << s << EOL;
  }

  void warning(const String& s) {
    fout << "XML - warning: " << s << EOL;
  }

  void error(const String& s) {
    fout << "XML - error: " << s << EOL;
  }

  void fatalError(const String& s) {
    fout << "XML - fatalError: " << s << EOL;
  }
};

class StripCallback : public XMLParser::Callback {
private:

  bool ignore;
  Stack<bool> oldIgnores;
  Array<bool> a;
  unsigned int i;
  unsigned int chapter;
public:

  StripCallback() throw() : ignore(false), chapter(1) {
  }

  void startElement(const String& name, const XMLPropertyHash& attributes) {
//    oldIgnores.push(ignore);
    a.append(ignore);
    ignore = false;
    if (name == String("title")) {
    } else if (name == String("chapter")) {
      fout << "  " << chapter++ << ". ";
    } else if (name == String("para")) {
    } else { // unknown entry
      ignore = true;
    }
  }

  void endElement(const String& name) {
    if (!ignore) {
      fout << EOL;
    }
    ignore = a[a.getSize() - 1];
    a.remove(a.getSize() - 1);
  }

  void characters(const String& s) {
    if (!ignore) {
      fout << s;
    }
  }

  void warning(const String& s) {
    ferr << "XML - warning: " << s << EOL;
  }

  void error(const String& s) {
    ferr << "XML - error: " << s << EOL;
  }

  void fatalError(const String& s) {
    ferr << "XML - fatalError: " << s << EOL;
  }
};



void test(const String& filename) {
  fout << "Testing XML classes..." << ENDL;

  Allocator<char> buffer;

  {
    fout << "Reading XML file into buffer..." << ENDL;
    File file(filename, File::READ, File::EXCLUSIVE);
    buffer.setSize(file.getSize());
    file.read(buffer.getElements(), file.getSize());
  }

  fout << ">>>" << ENDL;
  for (unsigned int i = 0; i < buffer.getSize(); ++i) {
    fout << buffer.getElements()[i];
  }
  fout << "<<<" << ENDL;

  fout << "Parsing..." << ENDL;
  Callback callback;
  XMLParser parser(&callback);
  parser.parse(buffer.getElements(), buffer.getSize());
  parser.terminate();
}

int main(int argc, char* argv[]) {

  if (argc != 2) {
    fout << "xml filename" << ENDL;
    return 0; // stop
  }

  String filename = argv[1];

  try {
    test(filename);
  } catch(Exception& e) {
    ferr << "Catched exception - " << getTypename(e) << ": " << e.getMessage() << ENDL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
    return 1;
  }
  return 0;
}
