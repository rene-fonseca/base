/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/XMLReader.h>
#include <base/io/FileReader.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml/parser.h>
#  include <libxml/parserInternals.h>
#  include <stdarg.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/** Wrapper class for the callback interface. */
class XMLReaderImpl {
public:

  struct UserData {
    XMLReader* reader;
    void* context;
  };
  
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  static xmlEntityPtr getEntity(void* parser, const xmlChar* name) {
    return xmlGetPredefinedEntity(name);
  }

  static void entityDecl(
    void* parser,
    const xmlChar* name,
    int type,
    const xmlChar* publicId,
    const xmlChar* systemId,
    xmlChar* content) throw() {
    WRITE_SOURCE_LOCATION();
  }
  
  static void notationDecl(
    void* parser,
    const xmlChar* name,
    const xmlChar* publicId,
    const xmlChar* systemId) throw() {
    WRITE_SOURCE_LOCATION();
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {
      p->reader->dtdHandler->notationDecl(
        (const char*)name,
        (const char*)publicId,
        (const char*)systemId
      );
    }
  }
  
  static void attributeDecl(
    void* parser,
    const xmlChar* elementName,
    const xmlChar* name,
    int type,
    int def,
    const xmlChar* defaultValue,
    xmlEnumeration* tree) throw() {
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {

      AttributeDecl::ValueType valueType;
      switch (type) {
      case XML_ATTRIBUTE_CDATA:
        valueType = AttributeDecl::CDATA;
        break;
      case XML_ATTRIBUTE_ID:
        valueType = AttributeDecl::ID;
        break;
      case XML_ATTRIBUTE_IDREF:
        valueType = AttributeDecl::IDREF;
        break;
      case XML_ATTRIBUTE_IDREFS:
        valueType = AttributeDecl::IDREFS;
        break;
      case XML_ATTRIBUTE_ENTITY:
        valueType = AttributeDecl::ENTITY;
        break;
      case XML_ATTRIBUTE_ENTITIES:
        valueType = AttributeDecl::ENTITIES;
        break;
      case XML_ATTRIBUTE_NMTOKEN:
        valueType = AttributeDecl::NMTOKEN;
        break;
      case XML_ATTRIBUTE_NMTOKENS:
        valueType = AttributeDecl::NMTOKENS;
        break;
      case XML_ATTRIBUTE_ENUMERATION:
        valueType = AttributeDecl::ENUMERATION;
        break;
      case XML_ATTRIBUTE_NOTATION:
        valueType = AttributeDecl::NOTATION;
        break;
      default:
        throw SAXException();
      }

      AttributeDecl::DefaultType defaultType;
      switch (def) {
      case XML_ATTRIBUTE_NONE:
        defaultType = AttributeDecl::NONE;
        break;
      case XML_ATTRIBUTE_REQUIRED:
        defaultType = AttributeDecl::REQUIRED;
        break;
      case XML_ATTRIBUTE_IMPLIED:
        defaultType = AttributeDecl::IMPLIED;
        break;
      case XML_ATTRIBUTE_FIXED:        
        defaultType = AttributeDecl::FIXED;
        break;
      default:
        throw SAXException();
      }
      
      Array<String> enumeration;
      if (valueType == AttributeDecl::ENUMERATION) {
        while (tree) {
          enumeration.append((const char*)tree->name);
          tree = tree->next;
        }
      }
      p->reader->dtdHandler->attributeDecl(
        (const char*)name,
        (const char*)elementName,
        valueType,
        defaultType,
        (const char*)defaultValue,
        enumeration
      );
    }
  }
  
  static void elementDecl(
    void* parser,
    const xmlChar* name,
    int type,
    xmlElementContent* content) throw() {
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      
      ElementDecl::ValueType valueType;
      switch (type) {
      case XML_ELEMENT_TYPE_EMPTY:
        valueType = ElementDecl::EMPTY;
        break;
      case XML_ELEMENT_TYPE_ANY:
        valueType = ElementDecl::ANY;
        break;
      case XML_ELEMENT_TYPE_MIXED:
        valueType = ElementDecl::MIXED;
        break;
      case XML_ELEMENT_TYPE_ELEMENT:
        valueType = ElementDecl::ELEMENT;
        break;
      default:
        throw SAXException();
      }
      
      p->reader->dtdHandler->elementDecl((const char*)name, valueType);
    }
  }

  static void unparsedEntityDecl(
    void* parser,
    const xmlChar* name,
    const xmlChar* publicId,
    const xmlChar* systemId,
    const xmlChar* notationName) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {
      p->reader->dtdHandler->unparsedEntityDecl(
        (const char*)name,
        (const char*)publicId,
        (const char*)systemId,
        (const char*)notationName
      );
    }
  }
  
  static void startDocument(void* parser) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->startDocument();
    }
  }

  static void endDocument(void* parser) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->endDocument();
    }
  }
  
  static void startElement(
    void* parser, const xmlChar* name, const xmlChar** atts) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      if (atts) {
        unsigned int count = 0;
        for (const xmlChar** current = atts; *current; current += 2) {
          ++count;
        }
        Array<Attributes::Attribute> attributes;
        for (const xmlChar** current = atts; *current;) {
          attributes.append(
            Attributes::Attribute(
              Cast::pointer<const char*>(*current++),
              Cast::pointer<const char*>(*current++)
            )
          );
        }
        p->reader->contentHandler->startElement(
          String(Cast::pointer<const char*>(name)),
          attributes
        );
      } else {
        p->reader->contentHandler->startElement(
          String(Cast::pointer<const char*>(name)),
          Attributes()
        );
      }
    }
  }

  static void endElement(void* parser, const xmlChar* name) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->endElement(
        String(Cast::pointer<const char*>(name))
      );
    }
  }

  static void entityReference(void* parser, const xmlChar* name) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->entityReference(
        Cast::pointer<const char*>(name)
      );
    }
  }
  
  static void characters(void* parser, const xmlChar* ch, int length) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->characters(
        String(Cast::pointer<const char*>(ch), length)
      );
    }
  }
  
  static void ignorableWhitespace(
    void* parser, const xmlChar* ch, int length) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->ignorableWhitespace(
        String(Cast::pointer<const char*>(ch), length)
      );
    }
  }
  
  static void processingInstruction(
    void* parser, const xmlChar* target, const xmlChar* data) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->processingInstruction(
        Cast::pointer<const char*>(target),
        Cast::pointer<const char*>(data)
      );
    }
  }
  
  static void comment(void* parser, const xmlChar* value) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->comment(Cast::pointer<const char*>(value));
    }
  }
  
  static void cdataBlock(
    void* parser, const xmlChar* value, int length) throw() {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->cdataBlock(
        String(Cast::pointer<const char*>(value), length)
      );
    }
  }

  static void warning(void* parser, const char* fmt, ...) throw() {
    va_list arg;
    char buffer[512];
    va_start(arg, fmt);
    vsprintf(buffer, fmt, arg);
    va_end(arg);
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->errorHandler) {
      const char* end = find(buffer, sizeof(buffer), '\n');
      if (end) {
        buffer[end - buffer] = 0; // terminate
      }
      p->reader->errorHandler->warning(
        SAXParseException(
          buffer,
          Type::getType<XMLReader>(),
          Cast::pointer<const char*>(getPublicId(p->context)),
          Cast::pointer<const char*>(getSystemId(p->context)),
          getLineNumber(p->context),
          getColumnNumber(p->context)
        )
      );
    }
  }

  static void error(void* parser, const char* fmt, ...) throw() {
    va_list arg;
    char buffer[512];
    va_start(arg, fmt);
    vsprintf(buffer, fmt, arg);
    va_end(arg);
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->errorHandler) {
      const char* end = find(buffer, sizeof(buffer), '\n');
      if (end) {
        buffer[end - buffer] = 0; // terminate
      }
      p->reader->errorHandler->error(
        SAXParseException(
          buffer,
          Type::getType<XMLReader>(),
          Cast::pointer<const char*>(getPublicId(p->context)),
          Cast::pointer<const char*>(getSystemId(p->context)),
          getLineNumber(p->context),
          getColumnNumber(p->context)
        )
      );
    }
  }
  
  static void fatalError(void* parser, const char* fmt, ...) throw() {
    va_list arg;
    char buffer[512];
    va_start(arg, fmt);
    vsprintf(buffer, fmt, arg);
    va_end(arg);

    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->errorHandler) {
      const char* end = find(buffer, sizeof(buffer), '\n');
      if (end) {
        buffer[end - buffer] = 0; // terminate
      }
      p->reader->errorHandler->fatalError(
        SAXParseException(
          buffer,
          Type::getType<XMLReader>(),
          Cast::pointer<const char*>(getPublicId(p->context)),
          Cast::pointer<const char*>(getSystemId(p->context)),
          getLineNumber(p->context),
          getColumnNumber(p->context)
        )
      );
    }
  }
  
  static xmlSAXHandler SAX_HANDLER;
#endif
};

xmlSAXHandler XMLReaderImpl::SAX_HANDLER = {
  0, // internalSubset
  0, // isStandalone
  0, // hasInternalSubset
  0, // hasExternalSubset
  0, // resolveEntity
  XMLReaderImpl::getEntity, // getEntity
  XMLReaderImpl::entityDecl, // entityDecl
  XMLReaderImpl::notationDecl, // notationDecl
  XMLReaderImpl::attributeDecl, // attributeDecl
  XMLReaderImpl::elementDecl, // elementDecl
  XMLReaderImpl::unparsedEntityDecl, // unparsedEntityDecl
  0, // setDocumentLocator
  XMLReaderImpl::startDocument, // startDocument
  XMLReaderImpl::endDocument, // endDocument
  XMLReaderImpl::startElement, // startElement
  XMLReaderImpl::endElement, // endElement
  XMLReaderImpl::entityReference, // reference
  XMLReaderImpl::characters, // characters
  XMLReaderImpl::ignorableWhitespace, // ignorableWhitespace
  XMLReaderImpl::processingInstruction, // processingInstruction
  XMLReaderImpl::comment, // comment
  XMLReaderImpl::warning, // warning
  XMLReaderImpl::error, // error
  XMLReaderImpl::fatalError, // fatalError
  0, // getParameterEntity
  XMLReaderImpl::cdataBlock, // cdataBlock
  0, // externalSubset
  1 // initialized
};



void XMLReader::DTDHandler::attributeDecl(
  const String& name,
  const String& element,
  AttributeDecl::ValueType type,
  AttributeDecl::DefaultType defaultType,
  const String& defaultValue,
  const Array<String>& enumeration) throw() {
}

void XMLReader::DTDHandler::elementDecl(
  const String& name,
  ElementDecl::ValueType type) throw() {
}

void XMLReader::DTDHandler::entityDecl(
  const String& element,
  const String& name) throw() {
}

void XMLReader::DTDHandler::notationDecl(
  const String& name,
  const String& publicId,
  const String& systemId) throw() {
}

void XMLReader::DTDHandler::unparsedEntityDecl(
  const String& name,
  const String& publicId,
  const String& systemId,
  const String& notationName) throw() {
}

void XMLReader::ErrorHandler::error(
  const SAXParseException& exception) throw(SAXException) {
  throw exception;
}

void XMLReader::ErrorHandler::fatalError(
  const SAXParseException& exception) throw(SAXException) {
  throw exception;
}

void XMLReader::ErrorHandler::warning(
  const SAXParseException& exception) throw(SAXException) {
  throw exception;
}

void XMLReader::ContentHandler::startDocument() throw() {
}

void XMLReader::ContentHandler::endDocument() throw() {
}

void XMLReader::ContentHandler::startElement(
  const String& name,
  const Attributes& attributes) throw() {
}

void XMLReader::ContentHandler::endElement(const String& name) throw() {
}

void XMLReader::ContentHandler::processingInstruction(
  const String& target, const String& data) throw() {
}

void XMLReader::ContentHandler::characters(const String& value) throw() {
}

void XMLReader::ContentHandler::ignorableWhitespace(
  const String& value) throw() {
}

void XMLReader::ContentHandler::comment(const String& value) throw() {
}

void XMLReader::ContentHandler::cdataBlock(const String& value) throw() {
}

void XMLReader::ContentHandler::skippedEntity(const String& name) throw() {
}

void XMLReader::ContentHandler::entityReference(const String& name) throw() {
}

void XMLReader::ContentHandler::endPrefixMapping(
  const String& prefix) throw() {
}

void XMLReader::ContentHandler::startPrefixMapping(
  const String& prefix, const String& uri) throw() {
}

void XMLReader::ContentHandler::setDocumentLocator(Locator* locator) throw() {
}

XMLReader::XMLReader() throw()
  : contentHandler(0),
    dtdHandler(0),
    entityResolver(0),
    errorHandler(0),
    validation(true),
    terminated(false) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#else // no xml support
  throw SAXNotSupportedException(this);
#endif
}

bool XMLReader::getFeature(const String& name) const
  throw(SAXNotRecognizedException, SAXNotSupportedException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  // TAG: fixme
  int result = 0;
//   int code = xmlGetFeature(context, name.getElements(), &result);
//   assert(code == 0, SAXNotRecognizedException(this));
  return result != 0;
#else // no xml support
  throw SAXNotSupportedException(this);
#endif
}

void XMLReader::setFeature(const String& name, bool value)
  throw(SAXNotRecognizedException, SAXNotSupportedException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  // TAG: fixme
//   int temp = value ? 1 : 0;
//   int code = xmlSetFeature(context, name.getElements(), &value);
//   assert(code == 0, SAXNotRecognizedException(this));
#else // no xml support
  throw SAXNotSupportedException(this);
#endif
}

void XMLReader::parse(
  File file, const String& uri) throw(IOException, SAXException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  // xmlSubstituteEntitiesDefault((flags & XXX) ? 1 : 0); // TAG: fixme
  // xmlKeepBlanksDefault((flags & XXX) ? 1 : 0); // TAG: fixme
  // xmlPedanticParserDefault((flags & XXX) ? 1 : 0); // TAG: fixme
  // xmlLineNumbersDefault((flags & XXX) ? 1 : 0); // TAG: fixme
  long long size = file.getSize();
  assert(
    size >= 4,
    bindCause(
      SAXParseException("Document not well-formed", this),
      SAXParseException::NOT_WELL_FORMED
    )
  );
  FileReader reader(file, 0);
  reader.peek(4); // minimum number of bytes
  xmlParserCtxt* context = xmlCreatePushParserCtxt(
    &XMLReaderImpl::SAX_HANDLER,
    this,
    (const char*)reader.getBytes(),
    reader.getSize(),
    uri.isProper() ? uri.getElements() : 0
  );
  assert(context, SAXException("Unable to initialize parsing context", this));
  reader.skip(reader.getSize());
  int result = 0;
  long long position = 0;
  while ((position != size) && (result == 0)) {
    fout << "pos:" << position << " filesize:" << size << ENDL;
    result = xmlParseChunk(
      static_cast<xmlParserCtxtPtr>(context),
      (const char*)reader.getBytes(),
      reader.getSize(),
      0 // do not terminate
    );
    reader.skip(reader.getSize());
    position += reader.getSize();
  }
  if (result == 0) {
    result = xmlParseChunk(context, 0, 0, 1); // terminate
  }
  bool wellFormed = context->wellFormed;
  xmlFreeParserCtxt(context);
  if (wellFormed) {
    assert(result == 0, SAXException("Unable to parse buffer", this));
  } else {
    assert(
      result == 0,
      bindCause(
        SAXParseException("Document not well-formed", this),
        SAXParseException::NOT_WELL_FORMED
      )
    );
  }
#else // no xml support
  throw SAXNotSupportedException(this);
#endif
}

void XMLReader::parse(
  const char* buffer,
  unsigned int size,
  const String& uri) throw(SAXException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  assert(size >= 4, SAXParseException(this));
  xmlParserCtxt* context = xmlCreatePushParserCtxt(
    &XMLReaderImpl::SAX_HANDLER,
    this,
    buffer,
    size, // minimum 4 bytes
    uri.isProper() ? uri.getElements() : 0
  );
  assert(context, SAXException("Unable to initialize parsing context", this));
  int result = xmlParseChunk(context, 0, 0, 1); // terminate
  bool wellFormed = context->wellFormed;
  xmlFreeParserCtxt(context);
  if (wellFormed) {
    assert(result == 0, SAXParseException("Unable to parse buffer", this));
  } else {
    assert(
      result == 0,
      bindCause(
        SAXParseException("Document not well-formed", this),
        SAXParseException::NOT_WELL_FORMED
      )
    );
  }
#else // no xml support
  throw SAXNotSupportedException(this);
#endif
}

void XMLReader::parse(const String& systemId) throw(SAXException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlParserCtxt* context = xmlCreateFileParserCtxt(systemId.getElements());
  assert(context, SAXException("Unable to initialize parsing context", this));
  context->sax = &XMLReaderImpl::SAX_HANDLER;

  XMLReaderImpl::UserData userData = {this, context};
  context->userData = &userData;

//   {
//     int code;
//     int result = 0;
//     code = xmlGetFeature(context, "http://xml.org/sax/features/validation", &result);
//     fout << code << ' ' << "http://xml.org/sax/features/validation" << ' ' << (result != 0) << ENDL;
    
//     code = xmlGetFeature(context, "http://xml.org/sax/features/namespaces", &result);
//     fout << code << ' ' << "http://xml.org/sax/features/namespaces" << ' ' << (result != 0) << ENDL;

//     code = xmlGetFeature(context, "http://xml.org/sax/features/namespace-prefixes", &result);
//     fout << code << ' ' << "http://xml.org/sax/features/namespace-prefixes" << ' ' << (result != 0) << ENDL;
//   }
  
  int result = xmlParseDocument(context);
  bool wellFormed = context->wellFormed;
  fout << "doc: " << context->myDoc << ENDL;
  fout << "valid: " << context->valid << ENDL;
  fout << "validate: " << context->validate << ENDL;
  fout << "dir: " << context->directory << ENDL;
  fout << "version: " << context->version << ENDL;
  fout << "encoding: " << context->encoding << ENDL;
  fout << "standalone: " << context->standalone << ENDL;
  fout << "wellformed: " << context->wellFormed << ENDL;
  
  xmlFreeParserCtxt(context);
  if (wellFormed) {
    assert(result == 0, SAXParseException("Unable to parse buffer", this));
  } else {
    assert(
      result == 0,
      bindCause(
        SAXParseException("Document not well-formed", this),
        SAXParseException::NOT_WELL_FORMED
      )
    );
  }
#else // no xml support
  throw SAXNotSupportedException(this);
#endif
}

// void XMLReader::terminate() throw() {
// #if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//   int result = xmlParseChunk(static_cast<xmlParserCtxtPtr>(context), 0, 0, 1);
//   assert(result == 0, SAXException("Unable to terminate parsing"));
// #else // no xml support
// #endif
// }

XMLReader::~XMLReader() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
