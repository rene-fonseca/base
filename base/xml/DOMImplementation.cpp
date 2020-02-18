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
#include <base/xml/DOMImplementation.h>
#include <base/xml/XMLReader.h>
#include <base/UnitTest.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/tree.h>
#endif
#include <stdarg.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

extern String vstringf(const char* text, va_list);

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
class DOMImplementationImpl {
public:
  
  static void error(void* _context, const char* text, ...) noexcept
  {
    String* context = reinterpret_cast<String*>(_context);
    if (INLINE_ASSERT(context)) {
      va_list arg;
      va_start(arg, text);
      String buffer = vstringf(text, arg);
      va_end(arg);
      context->append(buffer);
    }
  }
};
#endif

bool DOMImplementation::isSupported() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  return true;
#else
  return false;
#endif
}

bool DOMImplementation::hasFeature(const String& name, const String& version) noexcept
{
  if (version.isProper() && (version != "2.0")) {
    return false;
  }
  if (name.compareToIgnoreCase("Core") == 0) {
    return true;
  }
  if (name.compareToIgnoreCase("XML") == 0) {
    return true;
  }
//   if (name.compareToIgnoreCase("Events") == 0) {
//     return true;
//   }
  return false;
}

Document DOMImplementation::createDocument(const String& version)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = xmlNewDoc(
    Cast::pointer<const xmlChar*>(version.getElements())
  );
  bassert(doc, DOMException(this));
  return doc;
#else
  _throw DOMException(this);
#endif
}

Document DOMImplementation::createDocument(
  DocumentType doctype,
  const String& version)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  bassert(
    Document(doctype.getOwnerDocument()).isInvalid(),
    bindCause(DOMException(this), DOMException::WRONG_DOCUMENT)
  );
  xmlDoc* doc = xmlNewDoc(
    Cast::pointer<const xmlChar*>(version.getElements())
  );
  bassert(doc, DOMException(this));
  if (doctype.isValid()) {
    String publicId = doctype.getPublicId();
    String systemId = doctype.getSystemId();
    xmlDtd* dtd = xmlCreateIntSubset(
      doc,
      Cast::pointer<const xmlChar*>(doctype.getName().getElements()),
      publicId.isProper() ? (const xmlChar*)(publicId.getElements()) : 0,
      systemId.isProper() ? (const xmlChar*)(systemId.getElements()) : 0
    );
    bassert(dtd, DOMException(this));
  }
  
  return doc;
#else
  _throw DOMException(this);
#endif
}

Document DOMImplementation::createDocument(
  const String& namespaceURI,
  const String& qualifiedName,
  DocumentType doctype,
  const String& version)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  bassert(
    Document(doctype.getOwnerDocument()).isInvalid(),
    bindCause(DOMException(this), DOMException::WRONG_DOCUMENT)
  );
  xmlDoc* doc = xmlNewDoc(
    Cast::pointer<const xmlChar*>(version.getElements())
  );
  bassert(doc, DOMException(this));
  if (doctype.isValid()) {
    String publicId = doctype.getPublicId();
    String systemId = doctype.getSystemId();
    xmlDtd* dtd = xmlCreateIntSubset(
      doc,
      Cast::pointer<const xmlChar*>(doctype.getName().getElements()),
      publicId.isProper() ? (const xmlChar*)(publicId.getElements()) : 0,
      systemId.isProper() ? (const xmlChar*)(systemId.getElements()) : 0
    );
    bassert(dtd, DOMException(this));
  }
  
  Document document(doc);
  if (namespaceURI.isProper()) {
    document.appendChild(
      document.createElementNS(namespaceURI, qualifiedName)
    );
  } else {
    document.appendChild(document.createElement(qualifiedName));
  }
  return document;
#else
  _throw DOMException(this);
#endif
}
  
Document DOMImplementation::createFromURI(
  const String& systemId, Mode mode, unsigned int flags)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  
  xmlDoValidityCheckingDefaultValue =
    (mode == DOMImplementation::VALIDATING) ? 1 : 0;
  
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
  
  String errorMessage;
  // TAG: init global variable in DOMImplementation constructor or other place
  xmlSetGenericErrorFunc(&errorMessage, DOMImplementationImpl::error);
  xmlDoc* doc = xmlParseFile(systemId.getElements());
  ferr << errorMessage << ENDL; // TAG: fixme
  bassert(doc, DOMException(this)); // TAG: use errorMessage
  return doc;
#else
  _throw DOMException(this);
#endif
}

Document DOMImplementation::createDocumentFromString(
  const String& value, Mode mode, unsigned int flags)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
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
  
  xmlDoc* result = nullptr;
	switch (mode) {
	case DOMImplementation::PARSING:
		xmlDoValidityCheckingDefaultValue = 0;
		result = xmlParseMemory(value.getElements(), value.getLength());
		break;
	case DOMImplementation::VALIDATING:
		xmlDoValidityCheckingDefaultValue = 1;
		result = xmlParseMemory(value.getElements(), value.getLength());
		break;
	case DOMImplementation::RECOVERING:
		xmlDoValidityCheckingDefaultValue = 0;
		result = xmlRecoverMemory(value.getElements(), value.getLength());
		break;
	}
  bassert(result, DOMException(this));
  return result;
#else
  _throw DOMException(this);
#endif
}

DocumentType DOMImplementation::createDocumentType(
  const String& qualifiedName,
  const String& publicId,
  const String& systemId)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDtd* node = xmlCreateIntSubset(
    0,
    Cast::pointer<const xmlChar*>(qualifiedName.getElements()),
    publicId.isProper() ? Cast::pointer<const xmlChar*>(publicId.getElements()) : 0,
    systemId.isProper() ? Cast::pointer<const xmlChar*>(systemId.getElements()) : 0
  );
  bassert(node, DOMException(this));
  return node;
#else
  _throw DOMException(this);
#endif
}

void DOMImplementation::saveDocument(
  Document document, const String& filename, bool indent)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int bytesWritten =
    xmlSaveFormatFile(filename.getElements(), doc, indent ? 1 : 0);
  bassert(bytesWritten >= 0, DOMException(this));
#else
  _throw DOMException(this);
#endif
}

void DOMImplementation::saveDocument(
  Document document,
  const String& filename,
  const String& encoding,
  bool indent)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int bytesWritten = xmlSaveFormatFileEnc(
    filename.getElements(),
    doc,
    encoding.getElements(),
    indent ? 1 : 0
  );
  bassert(bytesWritten >= 0, DOMException(this));
#else
  _throw DOMException(this);
#endif
}

String DOMImplementation::saveDocumentToMemory(
  Document document, bool spaces)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  // TAG: fixme - xmlIndentTreeOutput = 1 or xmlKeepBlanksDefault(0)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int size = 0;
  xmlChar* buffer = nullptr;
	xmlDocDumpFormatMemory(doc, &buffer, &size, spaces ? 1 : 0);
  bassert(buffer && (size > 0), DOMException(this));
  String result((const char*)buffer);
  xmlFree(buffer);
  return result;
#else
  _throw DOMException(this);
#endif
}

String DOMImplementation::saveDocumentToMemory(
  Document document, const String& encoding, bool spaces)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  // TAG: fixme - xmlIndentTreeOutput = 1 or xmlKeepBlanksDefault(0)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int size = 0;
  xmlChar* buffer = nullptr;
	xmlDocDumpFormatMemoryEnc(
    doc,
    &buffer,
    &size,
    encoding.getElements(),
    spaces ? 1 : 0
  );
  bassert(buffer && (size > 0), DOMException(this));
  String result((const char*)buffer);
  xmlFree(buffer);
  return result;
#else
  _throw DOMException(this);
#endif
}

#if defined(_COM_AZURE_DEV__BASE__TESTS) && defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)

class TEST_CLASS(DOMImplementation) : public UnitTest {
public:

  TEST_PRIORITY(200);
  TEST_PROJECT("base/xml");
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    DOMImplementation impl;
    Document document = impl.createDocument();
    TEST_ASSERT(document);

    ProcessingInstruction pi = document.createProcessingInstruction("xml-stylesheet href=\"style.css\"", "type=\"text/css\"");
    TEST_ASSERT(pi.getType() == Node::PROCESSING_INSTRUCTION_NODE);
    document.appendChild(pi);
    Element r = document.createElement("root");
    TEST_ASSERT(!r.getParent());
    document.appendChild(r);
    Comment c = document.createComment("This is a comment.");
    r.appendChild(c);
    Element p = document.createElement("person");
    Element n = document.createElement("name");
    n.setAttribute("first", "John");
    n.setAttribute("last", "Doe");
    p.appendChild(n);
    r.appendChild(p);
    Text t = document.createText("This is some text.");
    TEST_ASSERT(t.getType() == Node::TEXT_NODE);
    p.appendChild(t);

    CDATASection cdata = document.createCDATASection("Some text.");
    TEST_ASSERT(cdata.getType() == Node::CDATA_SECTION_NODE);

    TEST_ASSERT(r.getName() == "root");
    TEST_ASSERT(n.hasAttribute("first"));
    TEST_ASSERT(n.getParent().getName() == "person");
    TEST_ASSERT(n.getType() == Node::ELEMENT_NODE);

    // TEST_ASSERT(document.validate());

    String xml = impl.saveDocumentToMemory(document, true);
    TEST_ASSERT(xml);
    // fout << xml << ENDL;
  }
};

TEST_REGISTER(DOMImplementation);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
