/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/xml/XMLParser.h>
#include <gnome-xml/parser.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/** Wrapper class for the callback interface. */
class XMLParserImpl {
public:

  static xmlEntityPtr getEntity(void* parser, const xmlChar* n) {
    return xmlGetPredefinedEntity(n);
  };

  static void startDocument(void* parser) {
    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
      p->callback->startDocument();
    }
  };

  static void endDocument(void* parser) {
    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
      p->callback->endDocument();
    }
  };

  static void startElement(void* parser, const xmlChar* name, const xmlChar** atts) {
    XMLPropertyHash properties;

    for(const xmlChar** current = atts; current && *current; ++current) {
      String name((const char*) *current++);
      String value((const char*) *current);
      properties[name] = XMLProperty(name, value);
    }

    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
      p->callback->startElement(String((const char*)name), properties);
    }
  };

  static void endElement(void* parser, const xmlChar* n) {
    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
      p->callback->endElement(String((const char*)n));
    }
  };

  static void characters(void* parser, const xmlChar* s, int len) {
    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
//      p->callback->characters(String(n, len));
    }
  };

  static void comment(void* parser, const xmlChar* s) {
    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
//      p->callback->comment(String(s));
    }
  };

  static void warning(void* parser, const char *fmt, ...) {
  //  va_list arg;
  //  char buff[1024];
  //
  //  va_start(arg, fmt);
  //  vsprintf(buff, fmt, arg);
  //  va_end(arg);
  //

    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
      p->callback->warning(String()); // buff
    }
  };

  static void error(void* parser, const char *fmt, ...) {
  //  va_list arg;
  //  char buff[1024];
  //
  //  va_start(arg, fmt);
  //  vsprintf(buff, fmt, arg);
  //  va_end(arg);
  //
    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
      p->callback->error(String()); // buff
    }
  };

  static void fatalError(void* parser, const char *fmt, ...) {
  //  va_list arg;
  //  char buff[1024];
  //
  //  va_start(arg, fmt);
  //  vsprintf(buff, fmt, arg);
  //  va_end(arg);
  //
    XMLParser* p = static_cast<XMLParser*>(parser);
    if (p->callback) {
      p->callback->fatalError(String()); // buff
    }
  };
};



void XMLParser::prepare() throw(XMLException) {
  release();

  xmlSAXHandler saxHandler = {
    0, // internalSubset
    0, // isStandalone
    0, // hasInternalSubset
    0, // hasExternalSubset
    0, // resolveEntity
    XMLParserImpl::getEntity, // getEntity
    0, // entityDecl
    0, // notationDecl
    0, // attributeDecl
    0, // elementDecl
    0, // unparsedEntityDecl
    0, // setDocumentLocator
    XMLParserImpl::startDocument, // startDocument
    XMLParserImpl::endDocument, // endDocument
    XMLParserImpl::startElement, // startElement
    XMLParserImpl::endElement, // endElement
    0, // reference
    XMLParserImpl::characters, // characters
    0, // ignorableWhitespace
    0, // processingInstruction
    XMLParserImpl::comment, // comment
    XMLParserImpl::warning, // warning
    XMLParserImpl::error, // error
    XMLParserImpl::fatalError, // fatalError
    0, // getParameterEntity
    0 // cdataBlock
  };

  context = xmlCreatePushParserCtxt(&saxHandler, this, 0, 0, 0); // TAG: should size be >=4 here
  assert(context, XMLException("Unable to initialize parsing context"));
}

void XMLParser::release() throw() {
  if (context) {
    xmlFreeParserCtxt(static_cast<xmlParserCtxtPtr>(context)); // TAG: myDoc is not deleted
  }
}

XMLParser::XMLParser(Callback* cb = 0) throw(XMLException) : callback(cb), context(0) {
  prepare();
};

void XMLParser::parse(const char* buffer, unsigned int size) throw(XMLException) {
  int result = xmlParseChunk(static_cast<xmlParserCtxtPtr>(context), buffer, size, 0);
  assert(result == 0, XMLException("Unable to parse string"));
};

void XMLParser::terminate() throw() {
  int result = xmlParseChunk(static_cast<xmlParserCtxtPtr>(context), 0, 0, 1);
  assert(result == 0, XMLException("Unable to terminate parsing"));
};

XMLParser::~XMLParser() throw() {
  release();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
