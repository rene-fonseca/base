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
#include <base/io/File.h>
#include <base/xml/Document.h>
#include <base/xml/Stylesheet.h>
#include <base/xml/Transformer.h>
#include <base/collection/Stack.h>
#include <base/collection/Array.h>

using namespace dk::sdu::mip::base;

class XSLTProcessorApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  XSLTProcessorApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(MESSAGE("xsltprocessor"),
      numberOfArguments,
      arguments,
      environment) {
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ")
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
  
    if (arguments.getSize() != 3) {
      fout << getFormalName() << MESSAGE(" filename") << ENDL;
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
    
    fout << MESSAGE("Reading document") << ENDL;
    Document source(filename);
    source.save(filename + ".saved");
    
    fout << MESSAGE("Reading stylesheet...") << ENDL;
    Document xslDocument(arguments[1]);
    xslDocument.save(arguments[1] + ".saved");
    
    Stylesheet stylesheet(xslDocument);

    fout << MESSAGE("Stylesheet") << EOL
         << indent(2) << MESSAGE("output method: ")
         << stylesheet.getOutputMethod() << EOL
         << indent(2) << MESSAGE("namespace: ")
         << stylesheet.getNamespace() << EOL
         << indent(2) << MESSAGE("version: ")
         << stylesheet.getVersion() << EOL
         << indent(2) << MESSAGE("encoding: ")
         << stylesheet.getEncoding() << EOL
         << ENDL;

//     fout << MESSAGE("Excluded prefixes: ")
//          << stylesheet.getExcludedPrefixes() << ENDL;
    
    Transformer transformer;
    transformer.setParameter(
      MESSAGE("projectname"),
      MESSAGE("Base framework")
    );
    transformer.setStylesheet(stylesheet);
    
    fout << MESSAGE("Transforming document...") << ENDL;
    Document result = transformer.transform(source);
    
    fout << MESSAGE("Saving result document...") << ENDL;
    transformer.save(arguments[2], result);
  }
};

STUB(XSLTProcessorApplication);
