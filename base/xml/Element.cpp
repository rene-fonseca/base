/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/xml/Element.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String Element::getTagName() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  if (node->ns && node->ns->prefix) {
    return String((const char*)node->ns->prefix) + MESSAGE(":") +
      NativeString((const char*)node->name);
  } else {
    return NativeString((const char*)node->name);
  }
#else
  throw DOMException(this);
#endif
}

bool Element::hasAttributes() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  return node->properties;
#else
  throw DOMException(this);
#endif
}

Attribute Element::getFirstAttribute() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  return node->properties;
#else
  return 0;
#endif
}

bool Element::hasAttribute(const String& name) const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  xmlAttr* attribute = xmlHasProp(
    node,
    (const xmlChar*)name.getElements()
  );
  return attribute != 0;
#else
  throw DOMException(this);
#endif
}

String Element::getAttribute(const String& name) const throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  char* value = (char*)xmlGetProp(node, (const xmlChar*)name.getElements());
  assert(value, DOMException(this));
  String result(value);
  xmlFree(value);
  return result;
#else
  throw DOMException(this);
#endif
}

void Element::setAttribute(
  const String& name, const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  xmlAttr* attribute = xmlSetProp(
    node,
    (const xmlChar*)name.getElements(),
    (const xmlChar*)value.getElements()
  );
  assert(attribute, DOMException(this));
#else
  throw DOMException(this);
#endif
}

void Element::removeAttribute(const String& name) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  int result = xmlUnsetProp(
    node,
    (const xmlChar*)name.getElements()
  );
  assert(result == 0, DOMException(this));
#else
  throw DOMException(this);
#endif
}

Attribute Element::getAttributeNode(const String& name) throw(DOMException) {
  throw DOMException(this);
}

Attribute Element::setAttributeNode(Attribute attribute) throw(DOMException) {
  throw DOMException(this);
}

Attribute Element::removeAttributeNode(Attribute attribute) throw(DOMException) {
  throw DOMException(this);
}

Attribute Element::getAttributeNodeNS(
  const String& namespaceURI,
  const String& localName) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  throw DOMException(this);
//   xmlNode* node = (xmlNode*)getContext();
//   char* value = (char*)xmlGetNsProp(
//     node,
//     (const xmlChar*)localName.getElements(),
//     (const xmlChar*)namespaceURI.getElements(),
//   );
//   assert(value, DOMException(this));
//   String result(value);
//   xmlFree(value);
//   return result;
#else
  throw DOMException(this);
#endif
}

String Element::getAttributeNS(
  const String& namespaceURI, const String& localName) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  return NativeString(
    (const char*)xmlGetNsProp(
      node,
      (const xmlChar*)localName.getElements(),
      (const xmlChar*)namespaceURI.getElements()
    )
  );
#else
  throw DOMException(this);
#endif
}

void Element::setAttributeNS(
  const String& namespaceURI,
  const String& qualifiedName,
  const String& value) throw(DOMException) {
}

void Element::removeAttributeNS(
  const String& namespaceURI,
  const String& localName) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  int result = xmlUnsetNsProp(
    node,
    0, // TAG: fixme
    (const xmlChar*)localName.getElements()
  );
  assert(result, DOMException(this));
#else
  throw DOMException(this);
#endif
}

Attribute Element::setAttributeNodeNS(
  Attribute attribute) throw(DOMException) {
  throw DOMException(this);
}

bool Element::hasAttributeNS(
  const String& namespaceURI, const String& localName) const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlNode* node = (xmlNode*)getContext();
  xmlAttr* attribute = xmlHasNsProp(
    node,
    (const xmlChar*)localName.getElements(),
    (const xmlChar*)namespaceURI.getElements()
  );
  return attribute != 0;
#else
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
