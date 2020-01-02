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

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#  include <stdarg.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
class DOMImplementationImpl {
public:
  
  static void error(void* context, const char* message, ...) noexcept {    
    va_list arg;
    char buffer[4096]; // TAG: possible buffer overrun
    va_start(arg, message);
    vsprintf(buffer, message, arg);
    va_end(arg);

    // TAG: detect when the warning/error begins and ends
    // and filter important information
    
//     if (compare(message, "error:" ...) == 0) {
//     }
//     if (compare(message, "warning:" ...) == 0) {
//     }
//     if (compare(message, "validaty error:", ...) == 0) {
//     }
//     if (compare(message, "%s:%d" ...) == 0) {
//     }
//     if (compare(message, "%s", sizeof("%s")) == 0) {
//     }
//     else throw UnexpectedFailure();
    
    // fout << "123: " << message << ENDL;
    
//     const char* end = find(buffer, sizeof(buffer), '\n');
//     if (end) {
//       buffer[end - buffer] = 0; // terminate
//     }
    
    ((String*)context)->append(buffer);
  }
};
#endif

bool DOMImplementation::hasFeature(
  const String& name, const String& version) noexcept {
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

Document DOMImplementation::createDocument(
  const String& version) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  const String& version) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  const String& version) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  const String& systemId, Mode mode, unsigned int flags) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  
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
  const String& value, Mode mode, unsigned int flags) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  const String& systemId) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  Document document, const String& filename, bool indent) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  bool indent) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  Document document, bool spaces) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
  Document document, const String& encoding, bool spaces) {
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
