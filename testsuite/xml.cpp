/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/File.h>
#include <base/xml/XMLParser.h>
#include <base/collection/Stack.h>
#include <base/collection/Array.h>

using namespace dk::sdu::mip::base;

class XMLApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  XMLApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("xml"), numberOfArguments, arguments, environment) {
  }

  class Callback : public XMLParser::Callback {
  public:

    void startDocument() throw() {
      fout << MESSAGE("XML - startDocument") << EOL;
    }

    void endDocument() throw() {
      fout << MESSAGE("XML - endDocument") << EOL;
    }

    void startElement(const String& name, const XMLPropertyHash& attributes) throw() {
      fout << MESSAGE("XML - startElement: ") << name << MESSAGE(" - {");
      XMLPropertyHash::ReadEnumerator enu = attributes.getReadEnumerator();
      while (enu.hasNext()) {
        const XMLProperty* attribute = enu.next()->getValue();
        fout << '{' << attribute->getName() << ';' << attribute->getValue() << '}';
        if (enu.hasNext()) {
          fout << MESSAGE(", ");
        }
      }
      fout << '}' << EOL;
    }

    void endElement(const String& n) throw() {
      fout << MESSAGE("XML - endElement: ") << n << EOL;
    }

    void characters(const String& s) throw() {
      fout << MESSAGE("XML - characters: ") << s << EOL;
    }

    void comment(const String& s) throw() {
      fout << MESSAGE("XML - comment: ") << s << EOL;
    }

    void warning(const String& s) throw() {
      fout << MESSAGE("XML - warning: ") << s << EOL;
    }

    void error(const String& s) throw() {
      fout << MESSAGE("XML - error: ") << s << EOL;
    }

    void fatalError(const String& s) throw() {
      fout << MESSAGE("XML - fatalError: ") << s << EOL;
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

    void startElement(const String& name, const XMLPropertyHash& attributes) throw() {
//    oldIgnores.push(ignore);
      a.append(ignore);
      ignore = false;
      if (name == String("title")) {
      } else if (name == String("chapter")) {
        fout << MESSAGE("  ") << chapter++ << MESSAGE(". ");
      } else if (name == String("para")) {
      } else { // unknown entry
        ignore = true;
      }
    }

    void endElement(const String& name) throw() {
      if (!ignore) {
        fout << EOL;
      }
      ignore = a[a.getSize() - 1];
      a.remove(a.getSize() - 1);
    }

    void characters(const String& s) throw() {
      if (!ignore) {
        fout << s;
      }
    }

    void warning(const String& s) throw() {
      ferr << MESSAGE("XML - warning: ") << s << EOL;
    }

    void error(const String& s) throw() {
      ferr << MESSAGE("XML - error: ") << s << EOL;
    }

    void fatalError(const String& s) throw() {
      ferr << MESSAGE("XML - fatalError: ") << s << EOL;
    }
  };

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
  
    if (arguments.getSize() != 1) {
      fout << MESSAGE("xml filename") << ENDL;
      return; // stop
    }

    const String filename = arguments[0];
    
    Allocator<char> buffer;
    {
      fout << MESSAGE("Reading XML file into buffer...") << ENDL;
      File file(filename, File::READ, File::EXCLUSIVE);
      buffer.setSize(file.getSize());
      file.read(buffer.getElements(), file.getSize());
    }

    fout << MESSAGE(">>>") << ENDL;
    for (unsigned int i = 0; i < buffer.getSize(); ++i) {
      fout << buffer.getElements()[i];
    }
    fout << MESSAGE("<<<") << ENDL;

    fout << MESSAGE("Parsing...") << ENDL;
    Callback callback;
    XMLParser parser(&callback);
    parser.parse(buffer.getElements(), buffer.getSize());
    parser.terminate();
  }
};

STUB(XMLApplication);
