/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Timer.h>
#include <base/concurrency/Process.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/File.h>
#include <base/xml/XMLDefaultReader.h>
#include <base/xml/Document.h>
#include <base/xml/Entity.h>
#include <base/xml/Notation.h>
#include <base/collection/Array.h>
#include <base/mem/OwnershipPointer.h>
#include <base/string/StringOutputStream.h>
#include <base/string/WideString.h>

using namespace dk::sdu::mip::base;

bool debug = true;

class SAXBuilder : public XMLReader::DTDHandler,
                   public XMLReader::ContentHandler {
private:

  /** DOM implementation. */
  DOMImplementation dom;
  /** The document. */
  Document document;
  /** The current node. */
  Node current;
public:
  
  SAXBuilder() throw() {
  }

  Document getDocument() throw() {
    return document;
  }
  
  void startDTD(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
    DocumentType documentType = document.createAndSetDocumentType(
      name,
      publicId,
      systemId
    );
    // document.appendChild(documentType);
    // current = documentType;
  }

  void endDTD() throw() {
  }

  void startEntity(const String& name) throw() {
  }

  void endEntity(const String& name) throw() {
  }
  
  void attributeDecl(
    const String& name,
    const String& element,
    AttributeDecl::ValueType type,
    AttributeDecl::DefaultType defaultType,
    const String& defaultValue,
    const Array<String>& enumeration) throw() {
    fout << "attributeDecl: " << name << ' ' << type << ENDL;
  }
  
  void elementDecl(
    const String& name,
    ElementDecl::ValueType type) throw() {
    fout << "elementDecl: " << name << ' ' << type << ENDL;
  }
  
  void entityDecl(
    const String& element,
    const String& name) throw() {
    fout << "entityDecl: " << element << ' ' << name << ENDL;
  }
  
  void startDocument() throw() {
    // version, encoding, and standalone
    document = dom.createDocument(MESSAGE("1.0"));
    current = document;
  }
  
  void endDocument() throw() {
    // TAG: set: version, encoding, and standalone
  }
  
  void startElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName,
    const Attributes& attributes) throw() {
    Element element =
      namespaceURI.isProper() ? document.createElement(localName) :
      document.createElementNS(namespaceURI, qName);
    unsigned int size = attributes.getSize();
    for (unsigned int i = 0; i < size; ++i) {
      Attributes::Attribute attribute = attributes[i];
      element.setAttribute(attribute.getName(), attribute.getValue());
    }
    current.appendChild(element);
    current = element;
  }
  
  void endElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName) throw() {
    current = current.getParent();
  }
  
  void characters(const String& value) throw() {
    //current.appendChild(document.createTextNode(value));
  }
  
  void comment(const String& value) throw() {
    current.appendChild(document.createComment(value));
  }
  
  void cdataBlock(const String& value) throw() {
    current.appendChild(document.createCDATASection(value));
  }
  
  void processingInstruction(
    const String& target, const String& data) throw() {
    current.appendChild(
      document.createProcessingInstruction(target, data)
    );
  }
  
  void ignorableWhitespace(const String& value) throw() {
    // TAG: need preserve flag?
    // TAG: need IgnorableWhitespace node?
  }
  
  void skippedEntity(const String& name) throw() {
  }
  
  void startPrefixMapping(
    const String& prefix, const String& uri) throw() {
  }
  
  void endPrefixMapping(const String& prefix) throw() {
  }
  
  void setDocumentLocator(XMLReader::Locator* locator) throw() {
  }

  virtual ~SAXBuilder() throw() {
  }
};

class SAXCounter : public XMLReader::DTDHandler,
                   public XMLReader::ContentHandler {
private:
  
  MemorySize numberOfElements;
  MemorySize numberOfAttributes;
  MemorySize numberOfCharacters;
  MemorySize numberOfIgnorableWhitespaces;
  MemorySize numberOfProcessingInstructions;
public:
  
  SAXCounter() throw() {
    numberOfElements = 0;
    numberOfAttributes = 0;
    numberOfCharacters = 0;
    numberOfIgnorableWhitespaces = 0;
    numberOfProcessingInstructions = 0;
  }
  
  MemorySize getNumberOfElements() const throw() {
    return numberOfElements;
  }
  
  MemorySize getNumberOfAttributes() const throw() {
    return numberOfAttributes;
  }
  
  MemorySize getNumberOfCharacters() const throw() {
    return numberOfCharacters;
  }
  
  MemorySize getNumberOfIgnorableWhitespaces() const throw() {
    return numberOfIgnorableWhitespaces;
  }
  
  MemorySize getNumberOfProcessingInstructions() const throw() {
    return numberOfProcessingInstructions;
  }
  
  void notationDecl(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
  }

  void unparsedEntityDecl(
    const String& name,
    const String& publicId,
    const String& systemId,
    const String& notationName) throw() {
  }
  
  void attributeDecl(
    const String& name,
    const String& element,
    AttributeDecl::ValueType type,
    AttributeDecl::DefaultType defaultType,
    const String& defaultValue,
    const Array<String>& enumeration) throw() {
//     fout << indent(i)
//          << "attributeDecl("
//          << "name=" << quote(name) << ", "
//          << "element=" << quote(element) << ", "
//          << "type=" << type << ", "
//          << "defaultType=" << defaultType << ", "
//          << "enumeration={";
//     Array<String>::ReadEnumerator enu = enumeration.getReadEnumerator();
//     while (enu.hasNext()) {
//       fout << quote(*enu.next());
//       if (enu.hasNext()) {
//         fout << ',';
//       }
//     }
//     fout << '}' << ')' << EOL;
  }
  
  void elementDecl(
    const String& name,
    ElementDecl::ValueType type) throw() {
//     fout << indent(i)
//          << "elementDecl("
//          << "name=" << quote(name) << ", "
//          << "type=" << type << ')' << EOL;
  }
  
  void internalEntityDecl(
    const String& name,
    const String& text) throw() {
//     fout << indent(i)
//          << "internalEntityDecl("
//          << "name=" << quote(name) << ", "
//          << "text=" << quote(text) << EOL;
  }
  
  void externalEntityDecl(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
//     fout << indent(i)
//          << "externalEntityDecl("
//          << "name=" << quote(name) << ", "
//          << "publicId=" << quote(publicId) << ", "
//          << "systemId=" << quote(systemId) << ')' << EOL;
  }
  
  void startDocument() throw() {
    numberOfElements = 0;
    numberOfAttributes = 0;
    numberOfCharacters = 0;
    numberOfIgnorableWhitespaces = 0;
    numberOfProcessingInstructions = 0;
  }
  
  void endDocument() throw() {
  }
  
  void startElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName,
    const Attributes& attributes) throw() {
    numberOfElements++;
    numberOfAttributes += attributes.getSize();
  }
  
  void endElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName) throw() {
  }
  
  void characters(const String& value) throw() {
    numberOfCharacters += value.getLength();
  }
  
  void comment(const String& value) throw() {
  }
  
  void cdataBlock(const String& value) throw() {
    numberOfCharacters += value.getLength();
  }
  
  void processingInstruction(
    const String& target, const String& data) throw() {
    numberOfProcessingInstructions++;
  }
  
  void ignorableWhitespace(const String& value) throw() {
    numberOfIgnorableWhitespaces += value.getLength();
  }
  
  void skippedEntity(const String& name) throw() {
  }
  
  void startPrefixMapping(
    const String& prefix, const String& uri) throw() {
  }
  
  void endPrefixMapping(const String& prefix) throw() {
  }
  
  void setDocumentLocator(XMLReader::Locator* locator) throw() {
  }

  virtual ~SAXCounter() throw() {
  }
};

class SAXTracer : public XMLReader::DTDHandler,
                  public XMLReader::ContentHandler {
private:
  
  unsigned int i;
public:
  
  SAXTracer() throw() {
    i = 0;
  }

  static String quote(const String& value) throw() {
    WideString wide(value);
    
    StringOutputStream stream;
    stream << "\"";
    
    WideString::ReadIterator i = wide.getBeginReadIterator();
    WideString::ReadIterator end = wide.getEndReadIterator();

    while (i != end) {
      if (*i <= 127) { // DEL
        switch (*i) {
        case '\n':
          stream << "\\n";
          break;
        case '\r':
          stream << "\\r";
          break;
        case '\t':
          stream << "\\t";
          break;
        case '\\':
          stream << "\\\\";
          break;
        case '"':
          stream << "\\\"";
          break;
        default:
          if (ASCIITraits::isControl(*i)) {
            stream << "&#" << *i << ";";
          } else {
            stream << static_cast<char>(*i);
          }
        }
      } else {
        stream << "&#" << *i << ";";
      }
      ++i;
    }
    
    stream << "\"" << FLUSH;
    return stream.getString();
  }

  void comment(const String& data) throw() {
    fout << indent(i)
         << "comment(" << "data=" << quote(data) << ")" << EOL;
  }
  
  void startDTD(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
    fout << indent(i)
         << "startDTD("
         << "name=" << quote(name) << ", "
         << "publicId=" << quote(publicId) << ", "
         << "systemId=" << quote(systemId) << ')' << EOL;
    i++;
  }

  void endDTD() throw() {
    i--;
    fout << indent(i)
         << "endDTD()" << EOL;
  }

  void startEntity(const String& name) throw() {
    fout << indent(i)
         << "startEntity("
         << "name=" << quote(name) << ')' << EOL;
    i++;
  }

  void endEntity(const String& name) throw() {
    i--;
    fout << indent(i)
         << "endEntity("
         << "name=" << quote(name) << ')' << EOL;
  }

  void startCDATA() throw() {
    fout << indent(i)
         << "startCDATA()" << EOL;
    i++;
  }
  
  void endCDATA() throw() {
    i--;
    fout << indent(i)
         << "endCDATA()" << EOL;
  }
  
  void notationDecl(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
    fout << indent(i)
         << "notationDecl("
         << "name=" << quote(name) << ", "
         << "publicId=" << quote(publicId) << ", "
         << "systemId=" << quote(systemId) << ')' << EOL;
  }

  void unparsedEntityDecl(
    const String& name,
    const String& publicId,
    const String& systemId,
    const String& notationName) throw() {
    fout << indent(i)
         << "unparsedEntityDecl("
         << "name=" << quote(name) << ", "
         << "publicId=" << quote(publicId) << ", "
         << "systemId=" << quote(systemId) << ", "
         << "notationName=" << quote(notationName) << ')' << EOL;
  }
  
  void attributeDecl(
    const String& name,
    const String& element,
    AttributeDecl::ValueType type,
    AttributeDecl::DefaultType defaultType,
    const String& defaultValue,
    const Array<String>& enumeration) throw() {
    fout << indent(i)
         << "attributeDecl("
         << "name=" << quote(name) << ", "
         << "element=" << quote(element) << ", "
         << "type=" << type << ", "
         << "defaultType=" << defaultType << ", "
         << "enumeration={";
    Array<String>::ReadEnumerator enu = enumeration.getReadEnumerator();
    while (enu.hasNext()) {
      fout << quote(*enu.next());
      if (enu.hasNext()) {
        fout << ',';
      }
    }
    fout << '}' << ')' << EOL;
  }
  
  void elementDecl(
    const String& name,
    ElementDecl::ValueType type) throw() {
    fout << indent(i)
         << "elementDecl("
         << "name=" << quote(name) << ", "
         << "type=" << type << ')' << EOL;
  }
  
  void internalEntityDecl(
    const String& name,
    const String& text) throw() {
    fout << indent(i)
         << "internalEntityDecl("
         << "name=" << quote(name) << ", "
         << "text=" << quote(text) << ')' << EOL;
  }
  
  void externalEntityDecl(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
    fout << indent(i)
         << "externalEntityDecl("
         << "name=" << quote(name) << ", "
         << "publicId=" << quote(publicId) << ", "
         << "systemId=" << quote(systemId) << ')' << EOL;
  }
  
  void startDocument() throw() {
    i = 0;
    fout << indent(i)
         << "startDocument()" << EOL;
    i++;
  }
  
  void endDocument() throw() {
    i--;
    fout << indent(i)
         << "endDocument()" << EOL;
  }
  
  void startElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName,
    const Attributes& attributes) throw() {
    fout << indent(i)
         << "startElement("
         << "namespaceURI=" << namespaceURI << ',' << SP
         << "localName=" << localName << ',' << SP
         << "qName=" << qName << ',' << SP
         << "attributes=" << "{";
    unsigned int size = attributes.getSize();
    for (unsigned int index = 0; index < size; ++index) {
      Attributes::Attribute attribute = attributes[index];
      if (index > 0) {
        fout << ',';
      }
      fout << attribute.getName() << '=' << quote(attribute.getValue());
    }
    fout << "}" << ")" << EOL;
    i++;
  }
  
  void endElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName) throw() {
    i--;
    fout << indent(i)
         << "endElement("
         << "namespaceURI=" << namespaceURI << ',' << SP
         << "localName=" << localName << ',' << SP
         << "qName=" << qName << ")" << EOL;
  }
  
  void characters(const String& value) throw() {
    fout << indent(i)
         << "characters(" << "value=" << quote(value) << ")" << EOL;
  }  
  
  void cdataBlock(const String& value) throw() {
    fout << indent(i)
         << "cdataBlock(" << "value=" << quote(value) << ")" << EOL;
  }
  
  void processingInstruction(
    const String& target, const String& data) throw() {
    fout << indent(i)
         << "processingInstruction("
         << "target=" << quote(target) << ", "
         << "data=" << quote(data) << ")" << EOL;
  }
  
  void ignorableWhitespace(const String& value) throw() {
    fout << indent(i)
         << "ignorableWhitespace(" << "value=" << quote(value) << ")" << EOL;
  }
  
  void skippedEntity(const String& name) throw() {
    fout << indent(i)
         << "skippedEntity(" << "name=" << quote(name) << ")" << EOL;
  }
  
  void startPrefixMapping(
    const String& prefix, const String& uri) throw() {
    fout << indent(i)
         << "startPrefixMapping("
         << "prefix=" << quote(prefix) << ", "
         << "uri=" << quote(uri) << ")" << EOL;
    i++;
  }
  
  void endPrefixMapping(const String& prefix) throw() {
    i--;
    fout << indent(i)
         << "endPrefixMapping(" << quote(prefix) << ")" << EOL;
  }
  
  void setDocumentLocator(XMLReader::Locator* locator) throw() {
    fout << indent(i)
         << "setDocumentLocator(" << locator << ")" << EOL;
  }

  virtual ~SAXTracer() throw() {
  }
};

class SAXDumper : public XMLReader::DTDHandler,
                  public XMLReader::ContentHandler {
private:

  XMLReader* reader;
public:
  
  SAXDumper(XMLReader* _reader) throw()
    : reader(_reader) {
  }
  
  void startDTD(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
    fout << "<!DOCTYPE" << SP << name;
    if (publicId.isProper() && systemId.isProper()) {
      fout << SP << "PUBLIC" << SP << "\"" << publicId << "\"" << SP
           << "\"" << systemId << "\"";
    } else if (publicId.isProper()) {
      fout << SP << "PUBLIC" << SP << "\"" << publicId << "\"" ;
    } else {
      fout << SP << "SYSTEM" << SP << "\"" << systemId << "\"" ;
    }
    fout << SP << "[" << EOL;
  }
  
  void endDTD() throw() {
    fout << "]>" << EOL;
  }

  void startEntity(const String& name) throw() {
    // TAG: may be other format?
    fout << "&" << name << ";" << ENDL;
  }

  void endEntity(const String& name) throw() {
  }
  
  void notationDecl(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
    fout << "notationDecl("
         << "name=" << name << ", "
         << "publicId=" << publicId << ", "
         << "systemId=" << systemId << ')' << EOL;
  }

  void unparsedEntityDecl(
    const String& name,
    const String& publicId,
    const String& systemId,
    const String& notationName) throw() {
    fout << "unparsedEntityDecl("
         << "name=" << name << ", "
         << "publicId=" << publicId << ", "
         << "systemId=" << systemId << ", "
         << "notationName=" << notationName << ')' << EOL;
  }
  
  void attributeDecl(
    const String& name,
    const String& element,
    AttributeDecl::ValueType type,
    AttributeDecl::DefaultType defaultType,
    const String& defaultValue,
    const Array<String>& enumeration) throw() {
    fout << "attributeDecl("
         << "name=" << name << ", "
         << "element=" << element << ", "
         << "type=" << type << ", "
         << "defaultType=" << defaultType << ", "
         << "enumeration={";
    Array<String>::ReadEnumerator enu = enumeration.getReadEnumerator();
    while (enu.hasNext()) {
      fout << *enu.next();
      if (enu.hasNext()) {
        fout << ',';
      }
    }
    fout << '}' << ')' << EOL;
  }
  
  void elementDecl(
    const String& name,
    ElementDecl::ValueType type) throw() {
    fout << "elementDecl("
         << "name=" << name << ", "
         << "type=" << type << ')' << EOL;
  }
  
  void internalEntityDecl(
    const String& name,
    const String& text) throw() {
    fout << "<!ENTITY " << name << ' ' << '"' << text << '"' << '>' << EOL;
  }
  
  void externalEntityDecl(
    const String& name,
    const String& publicId,
    const String& systemId) throw() {
    fout << "<!ENTITY " << name << ' ';
    if (publicId.isProper() && systemId.isProper()) {
      fout << "PUBLIC"
           << ' ' << '"' << publicId << '"'
           << ' ' << '"' << systemId << '"';
    } else if (publicId.isProper()) {
      fout << "PUBLIC"
           << ' ' << '"' << publicId << '"';
    } else {
      fout << "SYSTEM"
           << ' ' << '"' << systemId << '"';      
    }
    fout << '>' << EOL;
  }
  
  void startDocument() throw() {
    fout << "<?xml"
         << ' ' << "version" << '=' << "\"" << "1.0" << "\""
         << ' ' << "encoding" << '=' << "\"" << "UTF-8" << "\"";
    if (reader->isStandalone()) {
      fout << ' ' << "standalone=\"yes\"";
    }
    fout << "?>" << EOL;
  }
  
  void endDocument() throw() {
  }
  
  void startElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName,
    const Attributes& attributes) throw() {
    fout << "<" << qName;
    unsigned int size = attributes.getSize();
    for (unsigned int index = 0; index < size; index++) {
      Attributes::Attribute attribute = attributes[index];
      fout << ' '
           << attribute.getName() << '=' << '"' << attribute.getValue() << '"';
    }
    fout << '>';
  }
  
  void endElement(
    const String& namespaceURI,
    const String& localName,
    const String& qName) throw() {
    fout << "</" << qName << ">";
  }
  
  void characters(const String& value) throw() {
    fout << value;
  }
  
  void comment(const String& value) throw() {
    fout << "<!--" << value << "-->";
  }
  
  void cdataBlock(const String& value) throw() {
    fout << "<![CDATA[" << value << "]]>";
  }
  
  void processingInstruction(
    const String& target, const String& data) throw() {
    fout << "<?" << target << SP << data << "?>" << EOL;
  }
  
  void ignorableWhitespace(const String& value) throw() {
    fout << value;
  }
  
  void skippedEntity(const String& name) throw() {
    fout << "skippedEntity(" << "name=" << name << ")" << EOL;
  }
  
  void startPrefixMapping(
    const String& prefix, const String& uri) throw() {
    fout << "startPrefixMapping("
         << "prefix=" << prefix << ", "
         << "uri=" << uri << ")" << EOL;
  }
  
  void endPrefixMapping(const String& prefix) throw() {
    fout << "endPrefixMapping(" << prefix << ")" << EOL;
  }
  
  void setDocumentLocator(XMLReader::Locator* locator) throw() {
    fout << "setDocumentLocator(" << locator << ")" << EOL;
  }

  virtual ~SAXDumper() throw() {
  }
};

class DOMToSAX : public XMLReader {
private:
  
  /** The registered content handler. */
  ContentHandler* contentHandler;
  /** The registered DTD handler. */
  DTDHandler* dtdHandler;
  /** The registered entity resolver. */
  EntityResolver* entityResolver;
  /** The registered error handler. */
  ErrorHandler* errorHandler;
  /** Specifies whether or not to validate the document. */
  bool validate;
  /** Specifies whether or not the parsing should be terminated. */
  bool terminated;
  /** Specifies whether or not the parsing is in progress. */
  bool parsing;
  /** Specified whether or not the document is standalone. */
  bool standalone;
protected:
  
  void traverseNode(Node node) throw(SAXException) {
    switch (node.getType()) {
    case Node::ELEMENT_NODE:
      {
        Element element = node;
        String namespaceURI = element.getNamespaceURI();
        String localName = element.getLocalName();
        String qName = element.getTagName();
        
        if (element.hasAttributes()) {
          Array<Attributes::Attribute> attributes;
          Attribute attribute = element.getFirstAttribute();
          while (attribute.isValid()) {
            attributes.append(
              Attributes::Attribute(
                attribute.getName(),
                attribute.getValue()
              )
            );
            attribute = attribute.getNextSibling();
          }
          contentHandler->startElement(
            namespaceURI,
            localName,
            qName,
            attributes
          );
        } else {
          contentHandler->startElement(
            namespaceURI,
            localName,
            qName,
            Attributes()
          );
        }
        
        for (
          Node child = element.getFirstChild();
          child.isValid();
          child = child.getNextSibling()
        ) {
          traverseNode(child);
        }
        
        contentHandler->endElement(
          namespaceURI,
          localName,
          qName
        );
      }
      break;
    case Node::TEXT_NODE:
      contentHandler->characters(Text(node).getData());
      break;
    case Node::CDATA_SECTION_NODE:
      dtdHandler->startCDATA();
      contentHandler->characters(CDATASection(node).getData());
      dtdHandler->endCDATA();
      // contentHandler->cdataBlock(CDATASection(node).getData());
      break;
    case Node::ENTITY_REFERENCE_NODE:
      {
        String name = node.getName();
        dtdHandler->startEntity(name);
        dtdHandler->endEntity(name);
      }
      break;
    case Node::ENTITY_NODE:
      {
        Entity entity = node;
        dtdHandler->unparsedEntityDecl(
          entity.getName(),
          entity.getPublicId(),
          entity.getSystemId(),
          entity.getNotationName()
        );
      }
      break;
    case Node::PROCESSING_INSTRUCTION_NODE:
      {
        ProcessingInstruction pi = node;
        contentHandler->processingInstruction(
          pi.getTarget(),
          pi.getData()
        );
      }
      break;
    case Node::COMMENT_NODE:
      contentHandler->comment(Comment(node).getData());
      break;
    case Node::DOCUMENT_TYPE_NODE:
      {
        DocumentType dtd = node;
        
        dtdHandler->startDTD(
          dtd.getName(),
          dtd.getPublicId(),
          dtd.getSystemId()
        );
        
        // entities
        for (
          Node child = node.getFirstChild();
          child.isValid();
          child = child.getNextSibling()) {
          WRITE_SOURCE_LOCATION();
          traverseNode(child);
        }
        
        dtdHandler->endDTD();
      }
      break;
    case Node::NOTATION_NODE:
      {
        Notation notation = node;
        dtdHandler->notationDecl(
          notation.getName(),
          notation.getPublicId(),
          notation.getSystemId()
        );
      }
      break;
    }
  }
public:

  /**
    Initializes the XML reader.
  */
  DOMToSAX() throw()
    : contentHandler(0),
      dtdHandler(0),
      entityResolver(0),
      errorHandler(0),
      validate(true),
      terminated(false),
      parsing(false),
      standalone(false) {
  }
  
  /**
    Returns the value of the specified feature.
  */
  bool getFeature(const String& name) const
    throw(SAXNotRecognizedException, SAXNotSupportedException) {
  }
  
  /**
    Sets the value of the specified feature.
  */
  void setFeature(const String& name, bool value)
    throw(SAXNotRecognizedException, SAXNotSupportedException) {
  }
  
  /**
    Returns the current content handler.
  */
  inline ContentHandler* getContentHandler() const throw() {
    return contentHandler;
  }
  
  /**
    Returns the current DTD handler.
  */
  inline DTDHandler* getDTDHandler() const throw() {
    return dtdHandler;
  }
  
  /**
    Returns the current entity resolver.
  */
  inline EntityResolver* getEntityResolver() const throw() {
    return entityResolver;
  }
  
  /**
    Returns the current error handler.
  */
  ErrorHandler* getErrorHandler() const throw() {
    return errorHandler;
  }
  
  /**
    Registers a content event handler.
  */
  inline void setContentHandler(ContentHandler* handler) throw() {
    contentHandler = handler;
  }
  
  /**
    Registers a DTD event handler.
  */
  inline void setDTDHandler(DTDHandler* handler) throw() {
    dtdHandler = handler;
  }
  
  /**
    Registers an entity resolver.
  */
  inline void setEntityResolver(EntityResolver* entityResolver) throw() {
    entityResolver = entityResolver;
  }
  
  /**
    Registers an error event handler.
  */
  inline void setErrorHandler(ErrorHandler* handler) throw() {
    errorHandler = handler;
  }

  /**
    Returns the validation flag.
  */
  inline bool getValidation() const throw() {
    return validate;
  }

  /**
    Sets the validation flag.
  */
  inline void setVatidation(bool validate) throw() {
    this->validate = validate;
  }

  /**
    Returns true if the document is a standalone document.
  */
  inline bool isStandalone() const throw() {
    return standalone;
  }
  
  /**
    Terminates the parsing.
  */
  void terminate() throw() {
  }
  
  /**
    Traverses the specified document.
  */
  void traverse(const Document& document) throw(SAXException) {
    assert(!parsing, SAXException(this));
    parsing = true;
    // TAG: fixme
    
    Node node = document;
    contentHandler->startDocument();
    for (
      Node child = node.getFirstChild();
      child.isValid();
      child = child.getNextSibling()) {
      traverseNode(child);
    }
    contentHandler->endDocument();
  }
  
  /**
    Destroys the XML reader.
  */
  ~DOMToSAX() throw() {
  }
};

class SAXApplication : public Application,
                       public XMLReader::ErrorHandler,
                       public XMLReader::EntityResolver {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  enum Command {
    COMMAND_PARSE,
    COMMAND_TRACE,
    COMMAND_COUNT,
    COMMAND_DUMP,
    COMMAND_BUILD_DOM
  };

  Command command;
  bool debug;
public:

  SAXApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("sax", numberOfArguments, arguments, environment) {
    command = COMMAND_BUILD_DOM;
    debug = false;
  }

  static String getLocationString(const SAXParseException& exception) throw() {
    StringOutputStream stream;
    String systemId = exception.getSystemId();
    if (systemId.isProper()) {
      stream << systemId << ':';
    }
    stream << exception.getLine() << ':' << exception.getColumn() << FLUSH;
    return stream.getString();
  }
  
  void error(const SAXParseException& exception) throw(SAXException) {
    fout << FLUSH;
    ferr << "Error: " << exception.getMessage()
         << " at "
         << SAXApplication::getLocationString(exception) << ENDL;
  }
  
  void fatalError(const SAXParseException& exception) throw(SAXException) {
    fout << FLUSH;
    ferr << "Fatal error: " << exception.getMessage()
         << " at "
         << SAXApplication::getLocationString(exception) << ENDL;
  }

  void warning(const SAXParseException& exception) throw(SAXException) {
    fout << FLUSH;
    ferr << "Warning: " << exception.getMessage()
         << " at "
         << SAXApplication::getLocationString(exception) << ENDL;
  }

  XMLReader::InputSource resolveEntity(
    const String& publicId, const String& systemId) throw(SAXException) {
    WRITE_SOURCE_LOCATION();
    return 0;
  }

  Document getBuiltinDocument() throw(DOMException) {
    DOMImplementation dom;
    
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
    return document;
  }
  
  void dumpDocument(Document document) throw() {
    document.save("sax.SAXBuilder.xml");
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
    if (arguments.getSize() != 1) {
      fout << getFormalName() << " uri" << ENDL;
      return; // stop
    }
    
    const String filename = arguments[0];
    
    XMLDefaultReader parser;
    parser.setErrorHandler(this);
    parser.setEntityResolver(this);

    try {
      parser.setFeature("http://xml.org/sax/features/validation", true);
    } catch (SAXException& e) {
    }
    
    try {
      parser.setFeature("http://xml.org/sax/features/namespaces", true);
    } catch (SAXException& e) {
    }
    
    try {
      parser.setFeature(
        "http://xml.org/sax/features/namespaces-prefixes",
        true
      );
    } catch (SAXException& e) {
    }
    
    // parser.setFeature("http://apache.org/xml/features/validation/schema", true);
    // parser.setFeature( "http://apache.org/xml/features/validation/schema-full-checking", true);
    // parser.setFeature("http://apache.org/xml/features/continue-after-fatal-error", true);
    
    command = COMMAND_DUMP;
    switch (command) {
    case COMMAND_BUILD_DOM:
      {
        SAXBuilder* builder = new SAXBuilder();
        parser.setDTDHandler(builder);
        parser.setContentHandler(builder);
        parser.parse(filename);
        dumpDocument(builder->getDocument());
        delete builder;
      }
      break;
    case COMMAND_COUNT:
      { 
        SAXCounter* counter = new SAXCounter();
        parser.setDTDHandler(counter);
        parser.setContentHandler(counter);
        Timer timer;
        Timer::ElapsedTime elapsedTime;
        
        Process::Times beforeTimes = Process::getProcess().getTimes();
        timer.start();
        parser.parse(filename);
        timer.stop();
        Process::Times afterTimes = Process::getProcess().getTimes();
        elapsedTime = (afterTimes.system + afterTimes.user) -
          (beforeTimes.system + beforeTimes.user);
        elapsedTime.roundToMicrosecond();
        
        fout << "Elapsed real time (H:M:S.microseconds): "
             << timer << EOL
             << "Elapsed processing time (H:M:S.microseconds): "
             << elapsedTime.getHours() << ':'
             << elapsedTime.getNMinutes() << ':'
             << elapsedTime.getNSeconds() << '.'
             << setWidth(6) << ZEROPAD << elapsedTime.getNMicroseconds() << EOL
             << "Elements: "
             << counter->getNumberOfElements() << EOL
             << "Attributes: "
             << counter->getNumberOfAttributes() << EOL
             << "Characters: "
             << counter->getNumberOfCharacters() << EOL
             << "Ignorable whitespaces: "
             << counter->getNumberOfIgnorableWhitespaces() << EOL
             << "Processing instructions: "
             << counter->getNumberOfProcessingInstructions() << EOL
             << ENDL;
        delete counter;
      }
      break;
    case COMMAND_TRACE:
      {
        SAXTracer* tracer = new SAXTracer();
        parser.setDTDHandler(tracer);
        parser.setContentHandler(tracer);
        parser.parse(filename);
        delete tracer;
      }
      break;
    case COMMAND_DUMP:
      {
        DOMImplementation dom;
        Document document;
        if (filename.isProper()) {
          DOMImplementation::Mode mode = DOMImplementation::PARSING;
          // unsigned int flags = WARNINGS|PEDANTIC|DETECT_IDS|COMPLETE_ATTRIBUTE_LISTS)
          document = dom.createFromURI(filename, mode);
        } else {
          document = getBuiltinDocument();
        }

        fout << "Saving document..." << ENDL;
        document.save("sax.xml");
        
        DOMToSAX* parser = new DOMToSAX();
        
        SAXTracer* tracer = new SAXTracer();
        parser->setDTDHandler(tracer);
        parser->setContentHandler(tracer);
        parser->traverse(document);
        delete tracer;
        
//         SAXDumper* dumper = new SAXDumper(parser);
//         parser->setDTDHandler(dumper);
//         parser->setContentHandler(dumper);
//         parser->traverse(document);
//         delete dumper;
        
//         SAXDumper* dumper = new SAXDumper(&parser);
//         parser.setDTDHandler(dumper);
//         parser.setContentHandler(dumper);
//         parser.parse(filename);
//         delete dumper;
      }
      break;
    default:
//     File file(filename, File::READ, 0);
//     parser.parse(file);
      parser.parse(filename);
    }
  }
};

STUB(SAXApplication);
