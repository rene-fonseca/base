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
#include <base/xml/DOMImplementation.h>
#include <base/xml/XMLReader.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//#  include <libxml/xmlmemory.h>
//#  include <libxml/xmlIO.h>
//#  include <libxml/xinclude.h>
#  include <libxml/tree.h>
//#  include <libxml/hash.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class DOMImplementationImpl : public XMLReader::ErrorHandler,
                              public XMLReader::ContentHandler {
private:

  // Document document;
  /** The root of the tree. */
  Node root;
  /** The current node. */
  Node current;
public:
  
};

bool DOMImplementation::hasFeature(
  const String& name, const String& version) throw() {
  if (version.isProper() && (version != "2.0")) {
    return false;
  }
  if (name.compareToIgnoreCase("Core") == 0) {
    return true;
  }
  return false;
}

Document DOMImplementation::createDocument(
  const String& version) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = xmlNewDoc(
    Cast::pointer<const xmlChar*>(version.getElements())
  );
  assert(doc, DOMException(this));
  return doc;
#else
  throw DOMException(this);
#endif
}

Document DOMImplementation::createFromURI(
  const String& systemId) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = xmlParseFile(systemId.getElements());
  assert(doc, DOMException(this));
  return doc;
#else
  throw DOMException(this);
#endif
}

Document DOMImplementation::createDocument(
  const String& namespaceURI,
  const String& qualifiedName,
  const DocumentType& doctype) throw(DOMException) {
  return 0; // TAG: fixme
}

Document DOMImplementation::createDocumentFromString(
  const String& value, Mode mode, unsigned int flags) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlSubstituteEntitiesDefault(
    (mode & DOMImplementation::SUBSTITUTE_ENTITIES) ? 1 : 0
  );
  
  xmlLoadExtDtdDefaultValue = 0;
	if (mode & DOMImplementation::DETECT_IDS) {
    xmlLoadExtDtdDefaultValue |= XML_DETECT_IDS;
  }
	if (mode & DOMImplementation::COMPLETE_ATTRIBUTE_LISTS) {
    xmlLoadExtDtdDefaultValue |= XML_COMPLETE_ATTRS;
  }
  
  xmlDoc* result = 0;
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
  assert(result, DOMException(this));
  return result;
#else
  throw DOMException(this);
#endif
}

void DOMImplementation::saveDocument(
  Document document, const String& filename, bool indent) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int bytesWritten =
    xmlSaveFormatFile(filename.getElements(), doc, indent ? 1 : 0);
  assert(bytesWritten >= 0, DOMException(this));
#else
  throw DOMException(this);
#endif
}

void DOMImplementation::saveDocument(
  Document document,
  const String& filename,
  const String& encoding,
  bool indent) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int bytesWritten = xmlSaveFormatFileEnc(
    filename.getElements(),
    doc,
    encoding.getElements(),
    indent ? 1 : 0
  );
  assert(bytesWritten >= 0, DOMException(this));
#else
  throw DOMException(this);
#endif
}

String DOMImplementation::saveDocumentToMemory(
  Document document, bool spaces) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  // TAG: fixme - xmlIndentTreeOutput = 1 or xmlKeepBlanksDefault(0)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int size = 0;
  xmlChar* buffer;
	xmlDocDumpFormatMemory(doc, &buffer, &size, spaces ? 1 : 0);
  assert(buffer && (size > 0), DOMException(this));
  String result((const char*)buffer);
  xmlFree(buffer);
  return result;
#else
  throw DOMException(this);
#endif
}

String DOMImplementation::saveDocumentToMemory(
  Document document, const String& encoding, bool spaces) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  // TAG: fixme - xmlIndentTreeOutput = 1 or xmlKeepBlanksDefault(0)
  xmlDoc* doc = (xmlDoc*)document.getContext();
	int size = 0;
  xmlChar* buffer;
	xmlDocDumpFormatMemoryEnc(
    doc,
    &buffer,
    &size,
    encoding.getElements(),
    spaces ? 1 : 0
  );
  assert(buffer && (size > 0), DOMException(this));
  String result((const char*)buffer);
  xmlFree(buffer);
  return result;
#else
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
