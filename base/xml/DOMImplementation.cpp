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

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//#  include <libxml/xmlmemory.h>
//#  include <libxml/xmlIO.h>
//#  include <libxml/xinclude.h>
#  include <libxml/tree.h>
//#  include <libxml/hash.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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

DocumentType DOMImplementation::createDocumentType(
  const String& qualifiedName,
  const String& publicId,
  const String& systemId) throw(DOMException) {
  return DocumentType(0); // TAG: fixme
}

Document DOMImplementation::createDocument(
  const String& namespaceURI,
  const String& qualifiedName,
  const DocumentType& doctype) throw(DOMException) {
  return Document(); // TAG: fixme
}
// Document::Document(const String& filename) throw(DOMException) {
// #if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//   xmlDoc* temp = xmlParseFile(filename.getElements());
//   assert(temp, DOMException(this));
//   document = new DocumentImpl(temp);
// #else
//   throw DOMException(this);
// #endif
// }

// void DOMImplementation::createDocumentFromURI(
//   Document document, const String& filename, bool indent) throw(DOMException) {
// #if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
//   xmlDoc* doc = (xmlDoc*)document.document->getContext();
//   // int bytesWritten = xmlSaveFile(filename.getElements(), doc);
// 	int bytesWritten =
//     xmlSaveFormatFile(filename.getElements(), doc, indent ? 1 : 0);
//   assert(bytesWritten >= 0, DOMException(this));
// #else
//   throw DOMException(this);
// #endif
// }

Document DOMImplementation::createDocument(
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
  Document document;
  document.document = new Document::DocumentImpl(result);
  return document;
#else
  throw DOMException(this);
#endif
}

void DOMImplementation::saveDocument(
  Document document, const String& filename, bool indent) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document.document->getContext();
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
  xmlDoc* doc = (xmlDoc*)document.document->getContext();
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
  xmlDoc* doc = (xmlDoc*)document.document->getContext();
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
  xmlDoc* doc = (xmlDoc*)document.document->getContext();
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
