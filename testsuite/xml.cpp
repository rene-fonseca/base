/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

using namespace base;

class XMLApplication : public Application {
public:

  XMLApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("xml"), numberOfArguments, arguments, environment) {
  }

  class Callback : public XMLParser::Callback {
  public:

    void startDocument() {
      fout << MESSAGE("XML - startDocument") << EOL;
    }

    void endDocument() {
      fout << MESSAGE("XML - endDocument") << EOL;
    }

    void startElement(const String& name, const XMLPropertyHash& attributes) {
      fout << MESSAGE("XML - startElement: ") << name << MESSAGE(" - {");
      XMLPropertyHash::ReadEnumerator enu = attributes.getReadEnumerator();
      while (enu.hasNext()) {
        const XMLProperty* attribute = enu.next()->getValue();
        fout << '{' << attribute->getName() << ';' << attribute->getValue() << '}';
        if (enu.hasNext()) {
          fout << ", ";
        }
      }
      fout << '}' << EOL;
    }

    void endElement(const String& n) {
      fout << MESSAGE("XML - endElement: ") << n << EOL;
    }

    void characters(const String& s) {
      fout << MESSAGE("XML - characters: ") << s << EOL;
    }

    void comment(const String& s) {
      fout << MESSAGE("XML - comment: ") << s << EOL;
    }

    void warning(const String& s) {
      fout << MESSAGE("XML - warning: ") << s << EOL;
    }

    void error(const String& s) {
      fout << MESSAGE("XML - error: ") << s << EOL;
    }

    void fatalError(const String& s) {
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
      ferr << MESSAGE("XML - warning: ") << s << EOL;
    }

    void error(const String& s) {
      ferr << MESSAGE("XML - error: ") << s << EOL;
    }

    void fatalError(const String& s) {
      ferr << MESSAGE("XML - fatalError: ") << s << EOL;
    }
  };

  void main() throw() {
    fout << MESSAGE("Testing XML classes...") << ENDL;

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

int main(int argc, const char* argv[], const char* env[]) {
  XMLApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
