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
#include <base/xml/DOMImplementation.h>
#include <base/xml/Document.h>
#include <base/xml/Entity.h>
#include <base/xml/Notation.h>
#include <base/collection/Stack.h>
#include <base/collection/Array.h>

using namespace dk::sdu::mip::base;

class DOMApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  DOMApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("dom", numberOfArguments, arguments, environment) {
  }
  
  void dumpTree(const Node& node) throw() {
    switch (node.getType()) {
    case Node::ELEMENT_NODE:
      {
        Element element = node;
        if (node.hasChildNodes()) {
          fout << '<' << element.getTagName();
          if (element.hasAttributes()) {
            Attribute attribute = element.getFirstAttribute();
            while (attribute.isValid()) {
              fout << ' '
                   << attribute.getName() << '='
                   << '"' << attribute.getValue() << '"';
              attribute = attribute.getNextSibling();
            }
          }
          fout << '>';
          for (
            Node child = element.getFirstChild();
            child.isValid();
            child = child.getNextSibling()
          ) {
            dumpTree(child);
          }
          fout << '<' << '/' << element.getTagName() << '>';
        } else {
          fout << '<' << element.getTagName();
          if (element.hasAttributes()) {
            Attribute attribute = element.getFirstAttribute();
            while (attribute.isValid()) {
              fout << ' '
                   << attribute.getName() << '='
                   << '"' << attribute.getValue() << '"';
              attribute = attribute.getNextSibling();
            }
          }
          fout << '/' << '>';
        }
      }
      break;
    case Node::ATTRIBUTE_NODE:
      throw DOMException(this);
    case Node::TEXT_NODE:
      fout << Text(node).getData();
      break;
    case Node::CDATA_SECTION_NODE:
      fout << "<![CDATA[" << CDATASection(node).getData() << "]]>";
      break;
    case Node::ENTITY_REFERENCE_NODE:
      fout << "&" << node.getName() << ";";
      break;
    case Node::ENTITY_NODE:
      {
        Entity entity = node;
        fout << "<!ENTITY % " << entity.getNotationName();
        
        String publicId = entity.getPublicId();
        String systemId = entity.getSystemId();
        
        if (publicId.isProper()) {
          fout << " PUBLIC \"" << publicId << "\"";
        }
        if (systemId.isProper()) {
          fout << " \"" << systemId << "\"";
        }
        fout << EOL;
      }
      // TAG: unparsed entity ... NDATA notationName
      // <!ENTITY VERSION "0.9.1">
      break;
    case Node::PROCESSING_INSTRUCTION_NODE:
      {
        ProcessingInstruction pi = node;
        fout << "<?" << pi.getTarget() << ' ' << pi.getData() << "?>" << EOL;
      }
      break;
    case Node::COMMENT_NODE:
      fout << "<!--" << Comment(node).getData() << "-->";
      break;
    case Node::DOCUMENT_NODE:
      fout << "<?xml";
      if (true) {
        fout << " version=" << "\"1.0\"";
      }
      if (true) {
        fout << " encoding=" << "\"UTF-8\"";
      }
      if (true) {
        fout << " standalone=" << "\"yes\"";
      }
      fout << "?>" << EOL;
      for (
        Node child = node.getFirstChild();
        child.isValid();
        child = child.getNextSibling()) {
        dumpTree(child);
      }
      break;
    case Node::DOCUMENT_TYPE_NODE:

// <!ENTITY % ISOlat1 PUBLIC "ISO 8879:1986//ENTITIES Added Latin 1//EN"
//                           "C:/local/www.oasis-open.org/docbook/xml/4.2/ent/iso-lat1.ent">
// <!ENTITY % DocBookEBNFDTD PUBLIC "-//OASIS//DTD DocBook EBNF Module V1.0//EN"
//                                  "C:/local/www.oasis-open.org/docbook/xml/ebnf/1.0/dbebnf.dtd">
// <!ENTITY % DocBookMathMLDTD PUBLIC "-//OASIS//DTD DocBook MathML Module V1.0//EN"
//                                    "C:/local/www.oasis-open.org/docbook/xml/mathml/1.0/dbmathml.dtd">

// <!ENTITY gpl SYSTEM "gpl-appendix.xml">
// <!ENTITY fdl SYSTEM "fdl-appendix.xml">
// <!ENTITY bookindex SYSTEM "index.xml">

// <!ENTITY HOMEPAGE "https://www.fonseca.dk/base">
// <!ENTITY DISTRIBUTION "https://www.fonseca.dk/base/distribution">
// <!ENTITY NIGHTLY "https://www.fonseca.dk/base/distribution/base-framework/nightly">
// <!ENTITY PRERELEASE "https://www.fonseca.dk/base/distribution/base-framework/prerelease">
// <!ENTITY RELEASE "https://www.fonseca.dk/base/distribution/base-framework/release">
// <!ENTITY VERSION "0.9.1">
// %ISOlat1;

      {
        DocumentType dtd = node;
        fout << "<!DOCTYPE " << dtd.getName();
        
        String publicId = dtd.getPublicId();
        String systemId = dtd.getSystemId();
        
        if (publicId.isProper()) {
          fout << " PUBLIC \"" << publicId << "\"";
        }
        if (systemId.isProper()) {
          fout << " \"" << systemId << "\"";
        }
        fout << " [";
        
        // entities
        for (
          Node child = node.getFirstChild();
          child.isValid();
          child = child.getNextSibling()) {
          dumpTree(child);
        }
        
        fout << "]>" << EOL;
      }
      break;
    case Node::DOCUMENT_FRAGMENT_NODE:
      fout << "FRAGMENT" << ENDL; // TAG: fixme
      break;
    case Node::NOTATION_NODE:
      {
        Notation notation = node;
        fout << "<!NOTATION "; // TAG: << notation.getNotationName();
        
        String publicId = notation.getPublicId();
        String systemId = notation.getSystemId();
        
        if (publicId.isProper() && systemId.isProper()) {
          fout << "PUBLIC"
               << ' ' << "\"" << publicId << "\""
               << ' ' << "\"" << systemId << "\"";
        } else if (publicId.isProper()) {
          fout << "PUBLIC" << ' ' << "\"" << publicId << "\"";
        } else {
          fout << "SYSTEM" << ' ' << "\"" << systemId << "\"";
        }
        fout << '>' << EOL;
      }
      break;
    case Node::ATTRIBUTE_DECL_NODE:
      break;
    case Node::ELEMENT_DECL_NODE:
      break;
    case Node::ENTITY_DECL_NODE:
      break;
    }
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
  
    if (arguments.getSize() != 2) {
      fout << getFormalName() << " source destination" << ENDL;
      return; // stop
    }

    DOMImplementation dom;
    
    const String sourceName = arguments[0];
    const String destinationName = arguments[1];
    
    if (false) {
      Document document = dom.createDocument(MESSAGE("1.0"));
      
      document.appendChild(
        document.createProcessingInstruction(
          "xml-stylesheet",
          "type=\"text/xsl\" href=\"http://docbook.sourceforge.net/release/xsl/current/html/docbook.xsl\""
        )
      );

      DocumentType documentType = document.createAndSetDocumentType(
        "book",
        "-//OASIS//DTD DocBook XML V4.2//EN",
        "http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd"
      );
      // document.appendChild(documentType);
      
      Element book = document.createElement("book");
      book.setAttribute("status", "draft");
      document.appendChild(book);
      
      book.appendChild(document.createComment("This is a generated document"));
      
      Element chapter = document.createElement("chapter");
      book.appendChild(chapter);
      
      Element title = document.createElement("title");
      title.appendChild(document.createText("My First Chapter"));
      chapter.appendChild(title);
      
      Element para = document.createElement("para");
      chapter.appendChild(para);
      
      para.appendChild(document.createText("This is my paragraph "));
      para.appendChild(document.createCDATASection("<element>"));
      para.appendChild(document.createText("."));

      para.appendChild(document.createEntityReference("VERSION"));
      
      
      
      dumpTree(document);
      
      document.save(destinationName);
    } else {
      try {
        fout << "Reading document..." << ENDL;
        Document document = dom.createFromURI(sourceName);

        fout << "Validating document..." << ENDL;
        if (document.validate()) {
          fout << "Document is valid" << ENDL;
        } else {
          fout << "Document is not valid" << ENDL;
        }
        
        fout << "Saving result document..." << ENDL;
        document.save(destinationName);
      } catch(DOMException& e) {
        ferr << "Internal error: " << e << ENDL;
        setExitCode(EXIT_CODE_ERROR);
      }
    }
  }
};

STUB(DOMApplication);
