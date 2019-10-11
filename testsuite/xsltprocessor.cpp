/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

using namespace com::azure::dev::base;

class XSLTProcessorApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  XSLTProcessorApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("xsltprocessor",
      numberOfArguments,
      arguments,
      environment) {
  }

  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
  
    if (arguments.getSize() != 3) {
      fout << getFormalName() << " source destination" << ENDL;
      return; // stop
    }

    const String sourceName = arguments[0];
    const String destinationName = arguments[1];
    
    Allocator<uint8> buffer;
    {
       fout << "Reading XML file into buffer..." << ENDL;
       File file(sourceName, File::READ, File::EXCLUSIVE);
       buffer.setSize(file.getSize());
       file.read(buffer.getElements(), file.getSize());
    }

    DOMImplementation dom;
    
    fout << "Reading document" << ENDL;
    Document source = dom.createFromURI(sourceName);
    source.save(sourceName + ".saved");
    
    fout << "Reading stylesheet..." << ENDL;
    Document xslDocument = dom.createFromURI(destinationName);
    xslDocument.save(destinationName + ".saved");
    
    Stylesheet stylesheet(xslDocument);

    fout << "Stylesheet" << EOL
         << indent(2) << "output method: "
         << stylesheet.getOutputMethod() << EOL
         << indent(2) << "namespace: "
         << stylesheet.getNamespace() << EOL
         << indent(2) << "version: "
         << stylesheet.getVersion() << EOL
         << indent(2) << "encoding: "
         << stylesheet.getEncoding() << EOL
         << ENDL;

//     fout << "Excluded prefixes: "
//          << stylesheet.getExcludedPrefixes() << ENDL;
    
    Transformer transformer;
    transformer.setParameter(
      MESSAGE("projectname"),
      MESSAGE("Base framework")
    );
    transformer.setStylesheet(stylesheet);
    
    fout << "Transforming document..." << ENDL;
    Document result = transformer.transform(source);
    
    fout << "Saving result document..." << ENDL;
    transformer.save(arguments[2], result);
  }
};

STUB(XSLTProcessorApplication);
