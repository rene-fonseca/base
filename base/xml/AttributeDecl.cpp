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
#include <base/xml/AttributeDecl.h>

#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
#  include <libxml/tree.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

AttributeDecl::ValueType AttributeDecl::getValueType() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttribute* attribute = (xmlAttribute*)getContext();
  switch (attribute->atype) {
  case XML_ATTRIBUTE_CDATA:
    return AttributeDecl::CDATA;
  case XML_ATTRIBUTE_ID:
    return AttributeDecl::ID;
  case XML_ATTRIBUTE_IDREF:
    return AttributeDecl::IDREF;
  case XML_ATTRIBUTE_IDREFS:
    return AttributeDecl::IDREFS;
  case XML_ATTRIBUTE_ENTITY:
    return AttributeDecl::ENTITY;
  case XML_ATTRIBUTE_ENTITIES:
    return AttributeDecl::ENTITIES;
  case XML_ATTRIBUTE_NMTOKEN:
    return AttributeDecl::NMTOKEN;
  case XML_ATTRIBUTE_NMTOKENS:
    return AttributeDecl::NMTOKENS;
  case XML_ATTRIBUTE_ENUMERATION:
    return AttributeDecl::ENUMERATION;
  case XML_ATTRIBUTE_NOTATION:
    return AttributeDecl::NOTATION;
  default:
    throw DOMException(this);
  }
#else
  throw DOMException(this);
#endif
}

AttributeDecl::DefaultType AttributeDecl::getDefaultValueType() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttribute* attribute = (xmlAttribute*)getContext();
  switch (attribute->atype) {
  case XML_ATTRIBUTE_NONE:
    return AttributeDecl::NONE;
  case XML_ATTRIBUTE_REQUIRED:
    return AttributeDecl::REQUIRED;
  case XML_ATTRIBUTE_IMPLIED:
    return AttributeDecl::IMPLIED;
  case XML_ATTRIBUTE_FIXED:
    return AttributeDecl::FIXED;
  default:
    throw DOMException(this);
  }
#else
  throw DOMException(this);
#endif
}

String AttributeDecl::getDefaultValue() const throw() {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttribute* attribute = (xmlAttribute*)getContext();
  return (const char*)attribute->defaultValue;
#else
  throw DOMException(this);
#endif
}

Array<String> AttributeDecl::getEnumerationValues() const throw(DOMException) {
#if defined(_DK_SDU_MIP__BASE__XML_XMLSOFT_ORG)
  xmlAttribute* attribute = (xmlAttribute*)getContext();
  assert(
    attribute->atype == XML_ATTRIBUTE_ENUMERATION,
    bindCause(DOMException(this), DOMException::NOT_SUPPORTED)
  );
  Array<String> result;
  xmlEnumeration* e = attribute->tree;
  while (e) {
    result.append((const char*)e->name);
    e = e->next;
  }
  return result;
#else
  throw DOMException(this);
#endif
}

// TAG: namespace prefix
// TAG: element holding the attribute

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE