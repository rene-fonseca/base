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
#include <base/xml/AttributeDecl.h>

#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
#  include <libxml2/libxml/tree.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

AttributeDecl::ValueType AttributeDecl::getValueType() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
    _throw DOMException(this);
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

AttributeDecl::DefaultType AttributeDecl::getDefaultValueType() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
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
    _throw DOMException(this);
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

String AttributeDecl::getDefaultValue() const noexcept
{
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlAttribute* attribute = (xmlAttribute*)getContext();
  return NativeString((const char*)attribute->defaultValue);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

Array<String> AttributeDecl::getEnumerationValues() const
{
#if defined(_COM_AZURE_DEV__BASE__XML_XMLSOFT_ORG)
  xmlAttribute* attribute = (xmlAttribute*)getContext();
  bassert(
    attribute->atype == XML_ATTRIBUTE_ENUMERATION,
    bindCause(DOMException(this), DOMException::NOT_SUPPORTED)
  );
  Array<String> result;
  xmlEnumeration* e = attribute->tree;
  while (e) {
    result.append(NativeString((const char*)e->name));
    e = e->next;
  }
  return result;
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

// TAG: namespace prefix
// TAG: element holding the attribute

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
