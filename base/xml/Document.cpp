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
#include <base/xml/Document.h>
#include <base/Cast.h>
#include <base/string/FormatOutputStream.h> // REMOVE FIXME

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml/xmlmemory.h>
#  include <libxml/xmlIO.h>
#  include <libxml/xinclude.h>
#  include <libxml/tree.h>
#  include <libxml/hash.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Document::DocumentImpl::DocumentImpl() throw() {
  context = 0;
}

Document::DocumentImpl::DocumentImpl(void* context) throw() {
  this->context = context;
}

Document::DocumentImpl::~DocumentImpl() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  if (context) {
    xmlFreeDoc((xmlDocPtr)context);
  }
#endif
}

Document::Document() throw(DOMException) {
  document = new DocumentImpl();
}

void Document::create(const String& version) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = xmlNewDoc(
    Cast::pointer<const xmlChar*>(version.getElements())
  );
  assert(doc, DOMException(this));
  document = new DocumentImpl(doc);
#else
  throw DOMException(this);
#endif
}

DocumentType Document::getDocumentType() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlDtd* documentType = doc->intSubset;
  return documentType;
#else
  throw DOMException(this);
#endif
}

DOMImplementation Document::getImplementation() throw() {
  return DOMImplementation();
}

Element Document::getDocumentElement() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNode* node = xmlDocGetRootElement(doc);
  return node;
#else
  throw DOMException(this);
#endif
}

Attribute Document::createAttribute(const String& name) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlAttr* node = xmlNewDocProp(
    doc,
    Cast::pointer<const xmlChar*>(name.getElements()),
    0
  );
  assert(node, DOMException(this));
  return node;
#else
  throw DOMException(this);
#endif
}

Attribute Document::createAttribute(
  const String& name, const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  assert(doc, DOMException(this));
  xmlAttr* node = xmlNewDocProp(
    doc,
    Cast::pointer<const xmlChar*>(name.getElements()),
    Cast::pointer<const xmlChar*>(value.getElements())
  );
  assert(node, DOMException(this));
  return node;
#endif
}

Attribute Document::createAttributeNS(
  const String& namespaceURI,
  const String& qualifiedName) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  //  TAG: fixme
//   xmlDoc* doc = (xmlDoc*)document->getContext();
//   xmlAttr* node = xmlNewDocProp(
//     doc,
//     Cast::pointer<const xmlChar*>(name.getElements()),
//     0
//   );
//   assert(node, DOMException(this));
  //  return node;
  return 0;
#else
  throw DOMException(this);
#endif
}

Element Document::createElement(const String& name) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNode* node = xmlNewDocNode(
    doc,
    0, // namespace
    Cast::pointer<const xmlChar*>(name.getElements()),
    0 // content
  );
  assert(node, DOMException(this));
  return node;
#else
  throw DOMException(this);
#endif
}

Element Document::createElementNS(
  const String& namespaceURI,
  const String& qualifiedName) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNs* ns = xmlSearchNsByHref(
    doc,
    0,
    (const xmlChar*)namespaceURI.getElements()
  ); // TAG: check
  xmlNode* node = xmlNewNode(
    ns,
    Cast::pointer<const xmlChar*>(qualifiedName.getElements())
  );
  assert(node, DOMException(this));
  return node;
#else
  throw DOMException(this);
#endif
}

DocumentFragment Document::createDocumentFragment() throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNode* node = xmlNewDocFragment(doc);
  assert(node, DOMException(this));
  return node;
#else
  throw DOMException(this);
#endif
}

Text Document::createText(const String& data) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNode* node = xmlNewDocText(
    doc,
    Cast::pointer<const xmlChar*>(data.getElements())
  );
  assert(node, DOMException(this));
  return node;
#else
  throw DOMException(this);
#endif
}

Comment Document::createComment(const String& data) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNode* node = xmlNewComment(
    Cast::pointer<const xmlChar*>(data.getElements())
  );
  assert(node, DOMException(this));
  node->doc = doc;
  return node;
#else
  throw DOMException(this);
#endif
}

CDATASection Document::createCDATASection(
  const String& data) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  assert(doc, DOMException(this));
  xmlNode* node = xmlNewCDataBlock(
    doc,
    Cast::pointer<const xmlChar*>(data.getElements()),
    data.getLength()
  );
  assert(node, DOMException(this));
  node->doc = doc;
  return node;
#else
  throw DOMException(this);
#endif
}

ProcessingInstruction Document::createProcessingInstruction(
  const String& target,
  const String& data) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNode* node = xmlNewPI(
    Cast::pointer<const xmlChar*>(target.getElements()),
    Cast::pointer<const xmlChar*>(data.getElements())
  );
  assert(node, DOMException(this));
  node->doc = doc;
  return node;
#else
  throw DOMException(this);
#endif
}

EntityReference Document::createEntityReference(
  const String& name) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  xmlNode* node = xmlNewReference(
    doc,
    Cast::pointer<const xmlChar*>(name.getElements())
  );
  assert(node, DOMException(this));
  return node;
#else
  throw DOMException(this);
#endif
}

Element Document::getElementById(const String& elementId) throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  assert(doc, DOMException(this));
  void* node = xmlHashLookup(
    (xmlHashTable*)doc->ids,
    (const xmlChar*)elementId.getElements()
  );
  return node;
#else
  throw DOMException(this);
#endif
}

Node Document::importNode(Node importedNode, bool deep) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* doc = (xmlDoc*)document->getContext();
  assert(doc, DOMException(this));
  xmlNode* node = (xmlNode*)importedNode.context;

  switch (node->type) {
	case XML_ELEMENT_NODE:
	case XML_TEXT_NODE:
	case XML_CDATA_SECTION_NODE:
	case XML_ENTITY_REF_NODE:
	case XML_PI_NODE:
	case XML_COMMENT_NODE:
	case XML_DOCUMENT_FRAG_NODE:
    {
      xmlNode* result = xmlCopyNode(node, deep);
      xmlSetTreeDoc(result, doc);
      return result;
    }
  case XML_ATTRIBUTE_NODE:
    {
      if (doc->doc != doc) { // at root
        throw bindCause(DOMException(this), DOMException::NOT_SUPPORTED);
      }
      xmlNode* result = (xmlNode*)xmlCopyProp((xmlNode*)doc, (xmlAttr*)node);
      result->parent = 0;
      return result;
    }
  default:
		throw bindCause(DOMException(this), DOMException::NOT_SUPPORTED);
  }
#else
  throw DOMException(this);
#endif
}

Document::Document(const String& filename) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlDoc* temp = xmlParseFile(filename.getElements());
  assert(temp, DOMException(this));
  document = new DocumentImpl(temp);
#else
  throw DOMException(this);
#endif
}

void Document::doXIncludeProcess() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  int code = xmlXIncludeProcess(
    Cast::pointer<xmlDocPtr>(document->getContext())
  );
  assert(code >= 0, Exception(this));
#else
  throw DOMException(this);
#endif
}

void Document::save(const String& filename) throw(IOException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  int bytesWritten = xmlSaveFile(
    filename.getElements(),
    Cast::pointer<xmlDocPtr>(document->getContext())
  );
  assert(bytesWritten >= 0, Exception(this));
#else
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
