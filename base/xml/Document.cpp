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
#include <base/xml/Document.h>
#include <base/Cast.h>
#include <base/build.h>

#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
#  include <libxml/xmlmemory.h>
#  include <libxml/xmlIO.h>
#  include <libxml/xinclude.h>
#  include <libxml/tree.h>
#  include <libxml/hash.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

Document::Document()
{
}

DocumentType Document::getDocumentType() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlDtd* documentType = doc->intSubset;
  return documentType;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

DOMImplementation Document::getImplementation() noexcept
{
  return DOMImplementation();
}

Element Document::getDocumentElement() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlDocGetRootElement(doc);
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

DocumentType Document::createAndSetDocumentType(
  const String& qualifiedName,
  const String& publicId,
  const String& systemId)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  
  bassert(
    doc->intSubset == 0,
    bindCause(DOMException(this), DOMException::HIERARCHY_REQUEST)
  );
  
  xmlDtd* dtd = xmlCreateIntSubset(
    doc,
    (const xmlChar*)qualifiedName.getElements(),
    publicId.isProper() ? (const xmlChar*)publicId.getElements() : 0,
    systemId.isProper() ? (const xmlChar*)systemId.getElements() : 0
  );
  bassert(dtd, DOMException(this));
  return dtd;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Attribute Document::createAttribute(const String& name)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlAttr* node = xmlNewDocProp(
    doc,
    Cast::pointer<const xmlChar*>(name.getElements()),
    0
  );
  bassert(node, DOMException(this));
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Attribute Document::createAttribute(const String& name, const String& value)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  bassert(doc, DOMException(this));
  xmlAttr* node = xmlNewDocProp(
    doc,
    Cast::pointer<const xmlChar*>(name.getElements()),
    Cast::pointer<const xmlChar*>(value.getElements())
  );
  bassert(node, DOMException(this));
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Attribute Document::createAttributeNS(
  const String& namespaceURI,
  const String& qualifiedName)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  
  String prefix;
  String localName;
  xmlNs* ns = nullptr;
  
  int colon = qualifiedName.indexOf(":");
  if (colon) {
    prefix = qualifiedName.substring(0, colon);
    localName = qualifiedName.substring(colon + 1);
    bassert(
      prefix.isProper() &&
      localName.isProper() &&
      namespaceURI.isProper() &&
      ((prefix != "xml") ||
       (namespaceURI == "http://www.w3.org/XML/1998/namespace")) &&
      (((prefix != "xmlns") || (qualifiedName != "xmlns")) &&
       (namespaceURI == "http://www.w3.org/2000/xmlns")) &&
      (localName.indexOf(":") < 0),
      bindCause(DOMException("Malformed qualified name.", this), DOMException::NAMESPACE_ERROR)
    );
    
    ns = xmlNewNs( // TAG: destroy if exception is raised
      0,
      (const xmlChar*)namespaceURI.getElements(),
      (const xmlChar*)prefix.getElements()
    );
    bassert(ns, DOMException(this));
  } else {
    localName = qualifiedName;
    bassert(
      localName.isProper() && !namespaceURI.isProper(),
      bindCause(DOMException("Malformed qualified name.", this), DOMException::NAMESPACE_ERROR)
    );
  }

  // TAG: need FinalNode something for ns
  xmlAttr* attribute = xmlNewDocProp(
    doc,
    (const xmlChar*)localName.getElements(),
    0 // content
  );
  bassert(attribute, DOMException(this));
  if (ns) {
    xmlSetNs((xmlNode*)attribute, ns);
  }
  return attribute;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

#if 0
AttributeDecl Document::createAttributeDecl(
  const String& name,
  const String& element,
  AttributeDecl::ValueType valueType)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlNewDocNode(
    doc,
    0, // namespace
    Cast::pointer<const xmlChar*>(name.getElements()),
    0 // content
  );
  bassert(node, DOMException(this));
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

ElementDecl Document::createElementDecl(
  const String& name,
  const String& element,
  ElementDecl::ValueType valueType)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  static const xmlElementTypeVal
    VALUE_TYPES[ElementDecl::ELEMENT - ElementDecl::EMPTY + 1] = {
      XML_ELEMENT_TYPE_EMPTY, // ElementDecl::EMPTY
      XML_ELEMENT_TYPE_ANY, // ElementDecl::ANY
      XML_ELEMENT_TYPE_MIXED, // ElementDecl::MIXED
      XML_ELEMENT_TYPE_ELEMENT // ElementDecl::ELEMENT
    };
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlElement* element = xmlAddElementDecl(
    0, // xmlValidCtxt* context
    0, // xmlDtd* dtd
    (const xmlChar*)name.getElements(),
    VALUE_TYPES[valueType],
    0 // content
  );
  bassert(element, DOMException(this));
  return element;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Notation Document::createNotation(
  const String& name,
  const String& publicId,
  const String& systemId)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNotation* notation = xmlAddNotationDecl(
    0, // xmlValidCtxt* context
    0, // xmlDtd* dtd
    (const xmlChar*)name.getElements(),
    publicId.isProper() ? (const xmlChar*)publicId.getElements() : 0,
    privateId.isProper() ? (const xmlChar*)systemId.getElements() : 0
  );
  bassert(notation, DOMException(this));
  return notation;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}
#endif

Element Document::createElement(const String& name)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlNewDocNode(
    doc,
    0, // namespace
    Cast::pointer<const xmlChar*>(name.getElements()),
    0 // content
  );
  bassert(node, DOMException(this));
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Element Document::createElementNS(
  const String& namespaceURI,
  const String& qualifiedName)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();

  String prefix;
  String localName;
  int colon = qualifiedName.indexOf(":");
  if (colon) {
    prefix = qualifiedName.substring(0, colon);
    localName = qualifiedName.substring(colon + 1);
    bassert(
      prefix.isProper() &&
      localName.isProper() &&
      namespaceURI.isProper() &&
      ((prefix != "xml") ||
       (namespaceURI == "http://www.w3.org/XML/1998/namespace")) &&
        (localName.indexOf(":") < 0),
      bindCause(DOMException("Malformed qualified name.", this), DOMException::NAMESPACE_ERROR)
    );
  } else {
    localName = qualifiedName;
    bassert(
      localName.isProper(),
      bindCause(DOMException("Malformed qualified name.", this), DOMException::NAMESPACE_ERROR)
    );
  }
  
  xmlNs* ns = nullptr;
  if (prefix.isProper()) {
    ns = xmlNewNs(
      0,
      (const xmlChar*)namespaceURI.getElements(),
      (const xmlChar*)prefix.getElements()
    );
    bassert(ns, DOMException(this));
  }
  xmlNode* node = xmlNewDocNode(
    doc,
    ns,
    (const xmlChar*)localName.getElements(),
    0 // content
  );
  bassert(node, DOMException(this));  
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

DocumentFragment Document::createDocumentFragment()
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlNewDocFragment(doc);
  bassert(node, DOMException(this));
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Text Document::createText(const String& data)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlNewDocText(
    doc,
    Cast::pointer<const xmlChar*>(data.getElements())
  );
  bassert(node, DOMException(this));
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Comment Document::createComment(const String& data)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlNewComment(
    Cast::pointer<const xmlChar*>(data.getElements())
  );
  bassert(node, DOMException(this));
  node->doc = doc;
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

CDATASection Document::createCDATASection(
  const String& data)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  bassert(doc, DOMException(this));
  xmlNode* node = xmlNewCDataBlock(
    doc,
    Cast::pointer<const xmlChar*>(data.getElements()),
    data.getLength()
  );
  bassert(node, DOMException(this));
  node->doc = doc;
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

ProcessingInstruction Document::createProcessingInstruction(
  const String& target,
  const String& data)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlNewPI(
    Cast::pointer<const xmlChar*>(target.getElements()),
    Cast::pointer<const xmlChar*>(data.getElements())
  );
  bassert(node, DOMException(this));
  node->doc = doc;
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

EntityReference Document::createEntityReference(const String& name)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  xmlNode* node = xmlNewReference(
    doc,
    Cast::pointer<const xmlChar*>(name.getElements())
  );
  bassert(node, DOMException(this));
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Element Document::getElementById(const String& elementId)
{
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  bassert(doc, DOMException(this));
  void* node = xmlHashLookup(
    (xmlHashTable*)doc->ids,
    (const xmlChar*)elementId.getElements()
  );
  return node;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Node Document::importNode(Node importedNode, bool deep) {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* doc = (xmlDoc*)getContext();
  bassert(doc, DOMException(this));
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
      xmlNode* result = xmlCopyNode(node, deep ? 1 : 0);
      xmlSetTreeDoc(result, doc);
      return result;
    }
  case XML_ATTRIBUTE_NODE:
    {
      if (doc->doc != doc) { // at root
        _throw bindCause(DOMException(this), DOMException::NOT_SUPPORTED);
      }
      xmlNode* result = (xmlNode*)xmlCopyProp((xmlNode*)doc, (xmlAttr*)node);
      result->parent = 0;
      // TAG: specified flag suport be set to true on new attribute node
      return result;
    }
  default:
		throw bindCause(DOMException(this), DOMException::NOT_SUPPORTED);
  }
#else
  _throw DOMException(this);
#endif
}

void Document::doXIncludeSubstitution() {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  int code = xmlXIncludeProcess(
    Cast::pointer<xmlDoc*>(getContext())
  );
  bassert(code >= 0, DOMException(this));
#else
  _throw DOMException(this);
#endif
}

void Document::save(const String& filename) {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  int bytesWritten = xmlSaveFile(
    filename.getElements(),
    Cast::pointer<xmlDocPtr>(getContext())
  );
  bassert(bytesWritten >= 0, Exception(this));
#else
  _throw DOMException(this);
#endif
}

bool Document::validate() const {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlValidCtxt validateContext;
	validateContext.userData = 0;
	validateContext.error = 0;
	validateContext.warning = 0;
	return xmlValidateDocument(
    &validateContext,
    (xmlDoc*)getContext()
  ) == 1;
#else
  _throw DOMException(this);
#endif
}

bool Document::validate(Node node) const {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlValidCtxt validateContext;
	validateContext.userData = 0;
	validateContext.error = 0;
	validateContext.warning = 0;
	return xmlValidateElement(
    &validateContext,
    (xmlDoc*)getContext(),
    (xmlNode*)node.getContext()
  ) == 1;
#else
  _throw DOMException(this);
#endif
}

void Document::destroy() {
#if defined(_COM_AZURE_DEV__BASE__USE_XMLSOFT_XML)
  xmlDoc* document = (xmlDoc*)getContext();
  if (document) {
    xmlFreeDoc(document);
    context = 0;
  }
#else
  _throw DOMException(this);
#endif
}

__building_module

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
