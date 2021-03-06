/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/XMLDefaultReader.h>
#include <base/xml/DOMImplementation.h>
#include <base/xml/SAXNotSupportedException.h>
#include <base/io/FileReader.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  define LIBXML_LEGACY_ENABLED
#  include <libxml/SAX.h>
#  include <libxml/parser.h>
#  include <libxml/parserInternals.h>
#endif
#include <stdarg.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

extern String vstringf(const char* text, va_list);

/** Wrapper class for the callback interface. */
class XMLDefaultReaderImpl {
public:

  struct UserData {
    XMLDefaultReader* reader;
    void* context;
  };
  
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  static xmlEntityPtr getEntity(void* parser, const xmlChar* name) {
    return xmlGetPredefinedEntity(name);
  }

  static void internalSubset(
    void* parser,
    const xmlChar* name,
    const xmlChar* publicId,
    const xmlChar* systemId) noexcept {
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {
      p->reader->dtdHandler->startDTD(
        NativeString((const char*)name),
        NativeString((const char*)publicId),
        NativeString((const char*)systemId)
      );
      p->reader->dtdHandler->endDTD(); // TAG: where should this go
    }
  }
  
  static xmlParserInput* resolveEntity(
    void* parser,
    const xmlChar* publicId,
    const xmlChar* systemId)
  {
    fout << "resolveEntity:" << EOL
         << "publicId:" << String((const char*)publicId)
         << "systemId:" << String((const char*)systemId) << ENDL;
    
    UserData* p = static_cast<UserData*>(parser);
    xmlParserInput* result = xmlNewInputFromFile(
      (xmlParserCtxt*)p->context,
      (const char*)systemId
    );
    bassert(result, SAXException(Type::getType<XMLDefaultReader>()));
    return result;
  }
  
  static void entityDecl(
    void* parser,
    const xmlChar* name,
    int type,
    const xmlChar* publicId,
    const xmlChar* systemId,
    xmlChar* content)
  {
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {

      switch (type) {
      case XML_INTERNAL_GENERAL_ENTITY:
        p->reader->dtdHandler->internalEntityDecl(
          NativeString((const char*)name),
          NativeString((const char*)content)
        );
        break;
      case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
        p->reader->dtdHandler->externalEntityDecl(
          NativeString((const char*)name),
          NativeString((const char*)publicId),
          NativeString((const char*)systemId)
        );
        break;
      case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
        // TAG: is this case possible
        _throw SAXException();
        break;
      case XML_INTERNAL_PARAMETER_ENTITY:
        // TAG: use % prefix or other method
        p->reader->dtdHandler->internalEntityDecl(
          NativeString((const char*)name),
          NativeString((const char*)content)
        );
        break;
      case XML_EXTERNAL_PARAMETER_ENTITY:
        // TAG: use % prefix or other method
        p->reader->dtdHandler->externalEntityDecl(
          NativeString((const char*)name),
          NativeString((const char*)publicId),
          NativeString((const char*)systemId)
        );
        break;
      case XML_INTERNAL_PREDEFINED_ENTITY:
        p->reader->dtdHandler->internalEntityDecl(
          NativeString((const char*)name),
          NativeString((const char*)content)
        );
        break;
      default:
        return; // ignore
      }
    }
  }
  
  static void notationDecl(
    void* parser,
    const xmlChar* name,
    const xmlChar* publicId,
    const xmlChar* systemId) noexcept {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {
      p->reader->dtdHandler->notationDecl(
        NativeString((const char*)name),
        NativeString((const char*)publicId),
        NativeString((const char*)systemId)
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
    xmlEnumeration* tree)
  {
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {

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
        _throw SAXException();
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
        _throw SAXException();
      }
      
      Array<String> enumeration;
      if (valueType == AttributeDecl::ENUMERATION) {
        while (tree) {
          enumeration.append(NativeString((const char*)tree->name));
          tree = tree->next;
        }
      }
      p->reader->dtdHandler->attributeDecl(
        NativeString((const char*)name),
        NativeString((const char*)elementName),
        valueType,
        defaultType,
        NativeString((const char*)defaultValue),
        enumeration
      );
    }
  }
  
  static void elementDecl(
    void* parser,
    const xmlChar* name,
    int type,
    xmlElementContent* content)
  {
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {
      
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
        _throw SAXException();
      }
      
      p->reader->dtdHandler->elementDecl(
        NativeString((const char*)name),
        valueType
      );

      // TAG: how should element content be handled
    }
  }

  static void unparsedEntityDecl(
    void* parser,
    const xmlChar* name,
    const xmlChar* publicId,
    const xmlChar* systemId,
    const xmlChar* notationName) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->dtdHandler) {
      p->reader->dtdHandler->unparsedEntityDecl(
        NativeString((const char*)name),
        NativeString((const char*)publicId),
        NativeString((const char*)systemId),
        NativeString((const char*)notationName)
      );
    }
  }
  
  static void startDocument(void* parser) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->startDocument();
    }
  }

  static void endDocument(void* parser) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->endDocument();
    }
  }
  
  static void startElement(
    void* parser, const xmlChar* name, const xmlChar** atts) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      if (atts) {
        unsigned int count = 0;
        for (const xmlChar** current = atts; *current; current += 2) {
          ++count;
        }
        Array<Attributes::Attribute> attributes;
        for (const xmlChar** current = atts; *current; current += 2) {
          attributes.append(
            Attributes::Attribute(
              NativeString(Cast::pointer<const char*>(current[0])),
              NativeString(Cast::pointer<const char*>(current[1]))
            )
          );
        }
        p->reader->contentHandler->startElement(
          "", // namespaceURI // TAG: fixme
          String(Cast::pointer<const char*>(name)),
          "", // qName // TAG: fixme
          attributes
        );
      } else {
        p->reader->contentHandler->startElement(
          "", // namespaceURI // TAG: fixme
          String(Cast::pointer<const char*>(name)),
          "", // qName // TAG: fixme
          Attributes()
        );
      }
      // TAG: need to remember namespaces in hash table
      // HashTable<String, Namespace> uris;
      // HashTable<String, Namespace> prefixes;
    }
  }

  static void endElement(void* parser, const xmlChar* name) noexcept {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->endElement(
        "", // namespaceURI // TAG: fixme
        String(Cast::pointer<const char*>(name)), // localName
        "" // qName // TAG: fixme
      );
    }
  }

  static void entityReference(void* parser, const xmlChar* name) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->entityReference(
        NativeString(Cast::pointer<const char*>(name))
      );
    }
  }
  
  static void characters(void* parser, const xmlChar* ch, int length) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->characters(
        String(Cast::pointer<const char*>(ch), length)
      );
    }
  }
  
  static void ignorableWhitespace(
    void* parser, const xmlChar* ch, int length) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->ignorableWhitespace(
        String(Cast::pointer<const char*>(ch), length)
      );
    }
  }
  
  static void processingInstruction(
    void* parser, const xmlChar* target, const xmlChar* data) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->processingInstruction(
        NativeString(Cast::pointer<const char*>(target)),
        NativeString(Cast::pointer<const char*>(data))
      );
    }
  }
  
  static void comment(void* parser, const xmlChar* value) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->comment(
        NativeString(Cast::pointer<const char*>(value))
      );
    }
  }
  
  static void cdataBlock(
    void* parser, const xmlChar* value, int length) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->contentHandler) {
      p->reader->contentHandler->cdataBlock(
        String(Cast::pointer<const char*>(value), length)
      );
    }
  }

  static void warning(void* parser, const char* text, ...) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (!INLINE_ASSERT(p)) {
      return;
    }
    
#if 0
    va_list arg;
    va_start(arg, text);
    String buffer = vstringf(text, arg);
    va_end(arg);
#endif

    if (p->reader->errorHandler) {
#if 0
      const MemoryDiff index = buffer.indexOf('\n');
      if (index >= 0) {
        buffer = buffer.substring(0, index);
      }
#endif
      p->reader->errorHandler->warning(
        SAXParseException(
          "SAX parser warning.", // TAG: we need to copy string to use buffer,
          Type::getType<XMLDefaultReader>(),
          NativeString(Cast::pointer<const char*>(getPublicId(p->context))),
          NativeString(Cast::pointer<const char*>(getSystemId(p->context))),
          getLineNumber(p->context),
          getColumnNumber(p->context)
        )
      );
    }
  }

  static void error(void* parser, const char* text, ...) noexcept
  {
    UserData* p = static_cast<UserData*>(parser);
    if (!p) {
      return;
    }
    
#if 0
    va_list arg;
    va_start(arg, text);
    String buffer = vstringf(text, arg);
    va_end(arg);
#endif
    
    if (p->reader->errorHandler) {
#if 0
      const MemoryDiff index = buffer.indexOf('\n');
      if (index >= 0) {
        buffer = buffer.substring(0, index);
      }
#endif
      p->reader->errorHandler->error(
        SAXParseException(
          "SAX parser error.", // TAG: we need to copy string to use buffer,
          Type::getType<XMLDefaultReader>(),
          NativeString(Cast::pointer<const char*>(getPublicId(p->context))),
          NativeString(Cast::pointer<const char*>(getSystemId(p->context))),
          getLineNumber(p->context),
          getColumnNumber(p->context)
        )
      );
    }
  }
  
  static void fatalError(void* parser, const char* fmt, ...) noexcept
  {
#if 0
    va_list arg;
    va_start(arg, text);
    String buffer = vstringf(text, arg);
    va_end(arg);
#endif
    
    UserData* p = static_cast<UserData*>(parser);
    if (p->reader->errorHandler) {
#if 0
      const MemoryDiff index = buffer.indexOf('\n');
      if (index >= 0) {
        buffer = buffer.substring(0, index);
      }
#endif
      
      p->reader->errorHandler->fatalError(
        SAXParseException(
          "SAX parser fatal error.", // TAG: we need to copy string to use buffer,
          Type::getType<XMLDefaultReader>(),
          NativeString(Cast::pointer<const char*>(getPublicId(p->context))),
          NativeString(Cast::pointer<const char*>(getSystemId(p->context))),
          getLineNumber(p->context),
          getColumnNumber(p->context)
        )
      );
    }
  }
  
  static xmlSAXHandler SAX_HANDLER;
#endif
};

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
xmlSAXHandler XMLDefaultReaderImpl::SAX_HANDLER = {
  XMLDefaultReaderImpl::internalSubset, // internalSubset
  0, // isStandalone
  0, // hasInternalSubset
  0, // hasExternalSubset
  XMLDefaultReaderImpl::resolveEntity, // resolveEntity
  XMLDefaultReaderImpl::getEntity, // getEntity
  XMLDefaultReaderImpl::entityDecl, // entityDecl
  XMLDefaultReaderImpl::notationDecl, // notationDecl
  XMLDefaultReaderImpl::attributeDecl, // attributeDecl
  XMLDefaultReaderImpl::elementDecl, // elementDecl
  XMLDefaultReaderImpl::unparsedEntityDecl, // unparsedEntityDecl
  0, // setDocumentLocator
  XMLDefaultReaderImpl::startDocument, // startDocument
  XMLDefaultReaderImpl::endDocument, // endDocument
  XMLDefaultReaderImpl::startElement, // startElement
  XMLDefaultReaderImpl::endElement, // endElement
  XMLDefaultReaderImpl::entityReference, // reference
  XMLDefaultReaderImpl::characters, // characters
  XMLDefaultReaderImpl::ignorableWhitespace, // ignorableWhitespace
  XMLDefaultReaderImpl::processingInstruction, // processingInstruction
  XMLDefaultReaderImpl::comment, // comment
  XMLDefaultReaderImpl::warning, // warning
  XMLDefaultReaderImpl::error, // error
  XMLDefaultReaderImpl::fatalError, // fatalError
  0, // getParameterEntity
  XMLDefaultReaderImpl::cdataBlock, // cdataBlock
  0, // externalSubset
  1 // initialized
};
#endif



XMLDefaultReader::XMLDefaultReader() noexcept
  : contentHandler(0),
    dtdHandler(0),
    entityResolver(0),
    errorHandler(0),
    validate(true),
    terminated(false),
    parsing(false),
    standalone(false)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#else // no xml support
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

bool XMLDefaultReader::getFeature(const String& name) const
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  if (name == "http://xml.org/sax/features/validation") {
    return validate;
  } else if (name == "http://xml.org/sax/features/namespaces") {
    return false; // TAG: fixme
  } else if (name == "http://xml.org/sax/features/namespaces-prefixes") {
    return false; // TAG: fixme
  } else if (name == "http://xml.org/sax/features/is-standalone") {
    bassert(parsing, SAXNotSupportedException(this));
    return standalone;
  } else {
    _throw SAXNotRecognizedException(this);
  }
  
  // http://xml.org/sax/features/external-general-entities
  // http://xml.org/sax/features/external-parameter-entities
#else // no xml support
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void XMLDefaultReader::setFeature(const String& name, bool value)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  
  // TAG: what about recovering
  if (name == "http://xml.org/sax/features/validation") {
    validate = value;
  } else if (name == "http://xml.org/sax/features/namespaces") {
    _throw SAXNotSupportedException(this);
  } else if (name == "http://xml.org/sax/features/namespaces-prefixes") {
    _throw SAXNotSupportedException(this);
  } else {
    _throw SAXNotRecognizedException(this);
  }
#else // no xml support
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void XMLDefaultReader::parse(File file, const String& uri)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  bassert(!parsing, SAXException(this));
  parsing = true;
  
  unsigned int flags =
    DOMImplementation::WARNINGS|DOMImplementation::PEDANTIC|DOMImplementation::DETECT_IDS|DOMImplementation::COMPLETE_ATTRIBUTE_LISTS;
  // DOMImplementation::SUBSTITUTE_ENTITIES; TAG: fixme
  
  xmlDoValidityCheckingDefaultValue = validate ? 1 : 0;
  
  xmlGetWarningsDefaultValue =
    (flags & DOMImplementation::WARNINGS) ? 1 : 0;
  
  xmlSubstituteEntitiesDefault(
    (flags & DOMImplementation::SUBSTITUTE_ENTITIES) ? 1 : 0
  );
  xmlLoadExtDtdDefaultValue = 0;
	if (flags & DOMImplementation::DETECT_IDS) {
    xmlLoadExtDtdDefaultValue |= XML_DETECT_IDS;
  }
	if (flags & DOMImplementation::COMPLETE_ATTRIBUTE_LISTS) {
    xmlLoadExtDtdDefaultValue |= XML_COMPLETE_ATTRS;
  }
  xmlPedanticParserDefaultValue =
    (flags & DOMImplementation::PEDANTIC) ? 1 : 0;
  
  long long size = file.getSize();
  bassert(
    size >= 4,
    bindCause(SAXParseException("Document not well-formed.", this), SAXParseException::NOT_WELL_FORMED)
  );
  FileReader reader(file, 0);
  reader.peek(4); // minimum number of bytes
  xmlParserCtxt* context = xmlCreatePushParserCtxt(
    &XMLDefaultReaderImpl::SAX_HANDLER,
    this,
    (const char*)reader.getBytes(),
    reader.getSize(),
    uri.isProper() ? uri.getElements() : 0
  );
  bassert(context, SAXException("Unable to initialize parsing context.", this));
  reader.skip(reader.getSize());
  int result = 0;
  long long position = 0;
  while ((position != size) && (result == 0)) {
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
    bassert(result == 0, SAXException("Unable to parse buffer.", this));
  } else {
    bassert(
      result == 0,
      bindCause(SAXParseException("Document not well-formed.", this), SAXParseException::NOT_WELL_FORMED)
    );
  }
#else // no xml support
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void XMLDefaultReader::parse(
  const char* buffer,
  unsigned int size,
  const String& uri)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  bassert(!parsing, SAXException(this));
  parsing = true;
  
  bassert(size >= 4, SAXParseException(this));
  xmlParserCtxt* context = xmlCreatePushParserCtxt(
    &XMLDefaultReaderImpl::SAX_HANDLER,
    this,
    buffer,
    size, // minimum 4 bytes
    uri.isProper() ? uri.getElements() : 0
  );
  bassert(context, SAXException("Unable to initialize parsing context.", this));
  int result = xmlParseChunk(context, 0, 0, 1); // terminate
  bool wellFormed = context->wellFormed;
  xmlFreeParserCtxt(context);
  if (wellFormed) {
    bassert(result == 0, SAXParseException("Unable to parse buffer.", this));
  } else {
    bassert(
      result == 0,
      bindCause(SAXParseException("Document not well-formed.", this), SAXParseException::NOT_WELL_FORMED)
    );
  }
#else // no xml support
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

/**
  @short Automation object for setting a value on destruction.
*/
template<class TYPE>
class FinalValue : public AutomationObject {
private:
  
  TYPE& attribute;
  TYPE value;
public:
  
  /**
    Initializes the object.
    
    @param attribute The attribute.
    @param value The final value.
  */
  inline FinalValue(TYPE& _attribute, TYPE _value)
    : attribute(_attribute), value(_value)
  {
  }

  /**
    Destroys the automation object.
  */
  inline ~FinalValue()
  {
    attribute = value;
  }
};

void XMLDefaultReader::parse(const String& systemId)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  bassert(!parsing, SAXException(this));
  parsing = true;
  FinalValue<bool> finalValue(parsing, false);
  
  unsigned int flags =
    DOMImplementation::WARNINGS|DOMImplementation::PEDANTIC|DOMImplementation::DETECT_IDS|DOMImplementation::COMPLETE_ATTRIBUTE_LISTS;
  // DOMImplementation::SUBSTITUTE_ENTITIES; TAG: fixme
  
  xmlDoValidityCheckingDefaultValue = validate ? 1 : 0;
  
  xmlGetWarningsDefaultValue =
    (flags & DOMImplementation::WARNINGS) ? 1 : 0;
  
  xmlSubstituteEntitiesDefault(
    (flags & DOMImplementation::SUBSTITUTE_ENTITIES) ? 1 : 0
  );
  xmlLoadExtDtdDefaultValue = 0;
	if (flags & DOMImplementation::DETECT_IDS) {
    xmlLoadExtDtdDefaultValue |= XML_DETECT_IDS;
  }
	if (flags & DOMImplementation::COMPLETE_ATTRIBUTE_LISTS) {
    xmlLoadExtDtdDefaultValue |= XML_COMPLETE_ATTRS;
  }
  xmlPedanticParserDefaultValue =
    (flags & DOMImplementation::PEDANTIC) ? 1 : 0;
  
  xmlParserCtxt* context = xmlCreateFileParserCtxt(systemId.getElements());
  bassert(context, SAXException("Unable to initialize parsing context.", this));
  context->sax = &XMLDefaultReaderImpl::SAX_HANDLER;

  XMLDefaultReaderImpl::UserData userData = {this, context};
  context->userData = &userData;
  
  int result = xmlParseDocument(context);
  bool wellFormed = context->wellFormed;
// fout << "valid: " << context->valid << ENDL;
// fout << "validate: " << context->validate << ENDL;
// fout << "encoding: " << context->encoding << ENDL;
// fout << "standalone: " << context->standalone << ENDL;
  
  xmlFreeParserCtxt(context);
  if (wellFormed) {
    bassert(result == 0, SAXParseException("Unable to parse buffer.", this));
  } else {
    bassert(
      result == 0,
      bindCause(SAXParseException("Document not well-formed.", this), SAXParseException::NOT_WELL_FORMED)
    );
  }
#else // no xml support
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void XMLDefaultReader::terminate() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
// int result = xmlParseChunk(static_cast<xmlParserCtxtPtr>(context), 0, 0, 1);
// bassert(result == 0, SAXException("Unable to terminate parsing."));
#else // no xml support
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

XMLDefaultReader::~XMLDefaultReader() noexcept
{
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
