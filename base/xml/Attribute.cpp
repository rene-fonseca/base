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
#include <base/xml/Attribute.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String Attribute::getName() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttr* attribute = (xmlAttr*)getContext();
  if (attribute->ns) {
    const char* prefix = (const char*)attribute->ns->prefix;
    const char* name = (const char*)attribute->name;
    unsigned int length = 1; // ':'
    length += find<char>(prefix, String::MAXIMUM_LENGTH, 0) - prefix;
    length += find<char>(name, String::MAXIMUM_LENGTH, 0) - name;
    String result(length);
    result.append(prefix);
    result.append(MESSAGE(":"));
    result.append(name);
    return result;
  } else {
    return (const char*)attribute->name;
  }
#else
  throw DOMException(this);
#endif
}

String Attribute::getValue() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttr* attribute = (xmlAttr*)getContext();
  xmlChar* content = xmlNodeGetContent((xmlNode*)attribute);
  String result((const char*)content);
  xmlFree(content);
  return result;
#else
  throw DOMException(this);
#endif
}

void Attribute::setValue(const String& value) throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttr* attribute = (xmlAttr*)getContext();
  if (attribute->children) {
    xmlFreeNodeList(attribute->children);
  }
  attribute->children = 0;
  attribute->last = 0;
  
  xmlChar* encoded = xmlEncodeEntitiesReentrant(
    attribute->doc,
    (const xmlChar*)value.getElements()
  );
  attribute->children = xmlStringGetNodeList(attribute->doc, encoded);
  xmlNode* node = attribute->children;
  while (node) {
    node->parent = (xmlNode*)attribute;
    node->doc = attribute->doc;
    if (!node->next) {
      attribute->last = node;
    }
    node = node->next;
  }
  xmlFree(encoded);
#else
  throw DOMException(this);
#endif
}

bool Attribute::isSpecified() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttr* attribute = (xmlAttr*)getContext();
  return true; // TAG: fixme
#else
  throw DOMException(this);
#endif
}

Node::ShadowElement Attribute::getOwnerElement() throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttr* attribute = (xmlAttr*)getContext();
  return Node::ShadowElement(attribute->parent);
#else
  throw DOMException(this);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
